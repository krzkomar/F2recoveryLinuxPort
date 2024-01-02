#include "FrameWork.h"

int gItemUnk03[ 9 ] = { -1, 3, 3, 4, 4, 5, 0, 0, 0 };
int gItemUnk678[ 9 ] = { 0, 16, 17, 42, 41, 18, 45, 46, 47 };
int gItemClass[ 9 ] = { 0, 1, 1, 2, 2, 3, 4, 4, 4 };

Item01_t gItemGVar[ 9 ] = {
    { 0x06A, 0x015, 0 },
    { 0x057, 0x016, 4 },
    { 0x035, 0x017, 4 },
    { 0x06E, 0x018, 4 },
    { 0x030, 0x019, 0 },
    { 0x07C, 0x01A, 0 },
    { 0x07D, 0x01A, 0 },
    { 0x103, 0x128, 4 },
    { 0x130, 0x127, 0 }
};

char *gItemUnk02 = "<item>";
int gItemLightLvl = 0x10000;

Msg_t gItemMsg;
int gItemUnk99;
Obj_t *gItemUnk98;
int gItemGVarId;
int gItemGridLight[ 3*200*200 ];


/***********************************************************************/

int ItemInit()
{
    char stmp[260];

    if( MessageInit( &gItemMsg ) != 1 ) return -1;
    sprintf( stmp, "%s%s", gGamePath, "item.msg" );
    if( MessageLoad( &gItemMsg, stmp ) != 1 ) return -1;
    return 0;
}

int ItemSubNull0()
{
    return 0;
}

void ItemClose()
{
    MessageClose( &gItemMsg );
    ItemFSave( NULL );
}

int ItemFSave( xFile_t *fh )
{
    return 0;
}

int Item09( Obj_t *dude1, Obj_t *dude2, int quantity )
{
    int Type;
    Obj_t *Owner;
    Proto_t *proto;

    if( quantity < 1 ) return -1;
    Type = OBJTYPE( dude1->ImgId );
    if( Type == 1 ){
        if( CritterGetBodyType( dude1 ) ) return -5;
        if( ItemGetBackPackWeight( dude1 ) + ItemGetItemWeight( dude2 ) > FeatGetVal( dude1, FEAT_CARRY ) ) return -6;
    } else if( !Type ){
        Type = ItemGetObjType( dude1 );
        if( Type == 1 ){
            if( dude2 ) ProtoGetObj( dude2->Pid, &proto );
            if( Item95( dude1 ) > Item94( dude1 ) ) return -6;
            Owner = ObjGetOwner( dude1 );
            if( Owner ){
                if( OBJTYPE( Owner->ImgId ) == 1 ){
                    if( FeatGetVal( Owner, FEAT_CARRY ) < (ItemGetBackPackWeight( Owner ) + ItemGetItemWeight( dude2 )) ) return -6;
                }
            }
        } else {
            if( Type != 5 ) return -1;
            ProtoGetObj( dude1->Pid, &proto );
            if( proto->Critt.BaseStat[0] != dude2->Pid ) return -1;
        }
    }
    return ItemAdd(dude1, dude2, quantity);
}

int ItemAdd( Obj_t *dude, Obj_t *item, int Quantity )
{
    VidRect_t Area;
    int i,Ammo,aa,bb;
    ObjContainer_t *bpck;
    ObjStack_t *p;
    Proto_t *proto, *pObj;

    if( Quantity < 1 )return -1;
    bpck = &dude->Container;
    for( i = 0; i < bpck->Box.Cnt; i++ ){
        if( ItemStack( bpck->Box.Box[ i ].obj, item ) ) break;
    }
    if( i >= bpck->Box.Cnt ){
        if( bpck->Box.Cnt == bpck->Box.Capacity || !bpck->Box.Box ){            
            if( !(p = Realloc( bpck->Box.Box, sizeof( ObjStack_t ) * ( bpck->Box.Capacity + 10 ) ) ) ) return -1;
            bpck->Box.Box = p;
            bpck->Box.Capacity += 10;
        }
        bpck->Box.Box[ bpck->Box.Cnt ].obj = item;
        bpck->Box.Box[ bpck->Box.Cnt ].Quantity = Quantity;
        if( item->Pid == PID_STEALTHBOY && ( item->Flags & (PRFLG_WORN_LHAND | PRFLG_WORN_RHAND)) ){
            if( !(dude->Flags & 0x20000) ){
                dude->Flags |= 0x20000;
                ObjGetRadiusArea( dude, &Area );
                TileUpdateArea( &Area, dude->Elevation );
            }
        }
        bpck->Box.Cnt++;
        item->Owner = dude;
        return 0;
    } else if( item == bpck->Box.Box[ i ].obj ){
        eprintf( "Warning! Attempt to add same item twice in item_add()\n" );
        return 0;
    }
    if( ItemGetObjType(item) == 4 ){
        if( item ){
            ProtoGetObj( item->Pid, &proto );
            Ammo = item->Container.Ammo;
        } else {
            Ammo = 0;
        }
        aa = ItemGetAmmo( bpck->Box.Box[ i ].obj ) + Ammo;
        if( item ){
            ProtoGetObj( item->Pid, &pObj );
            bb = ( pObj->Critt.Type == 4 ) ? pObj->Critt.BaseStat[1] : pObj->Critt.BaseStat[15];
        } else {
            bb = 0;
        }
        if( aa > bb ){
            ItemSetCharges( item, aa - bb );
            bpck->Box.Box[ i ].Quantity++;
        } else {
            ItemSetCharges( item, aa );
        }
        bpck->Box.Box[ i ].Quantity += Quantity - 1;
    } else {
        bpck->Box.Box[ i ].Quantity += Quantity;
    }
    ObjDestroy( bpck->Box.Box[ i ].obj, 0 );
    bpck->Box.Box[ i ].obj = item;
    item->Owner = dude;
    return 0;
}

int ItemUseItem( Obj_t *Critter, Obj_t *Item, int QuantityMax )
{
    int i;
    ObjStack_t *stk;
    int SlotCapacity;
    Obj_t *Leader, *LHandObj, *RHandObj;

    LHandObj = InvGetLHandObj( Critter );
    RHandObj = InvGetRHandObj( Critter );
    if( Critter->Critter.Box.Cnt <= 0 ) return -1;
    for( i = 0; i < Critter->Container.Box.Cnt; i++ ){
        stk = &Critter->Critter.Box.Box[ i ];
        if( Item == stk->obj ){
	    SlotCapacity = stk->Quantity;
	    if( SlotCapacity < QuantityMax ) return -1;
	    if( SlotCapacity <= QuantityMax ){
    		i++;
    		for( ;i < Critter->Container.Box.Cnt; i++ ){
            	    Critter->Container.Box.Box[ i - 1].obj = Critter->Container.Box.Box[ i ].obj;
            	    Critter->Container.Box.Box[ i - 1].Quantity = Critter->Container.Box.Box[ i ].Quantity;
    		}
    		Critter->Container.Box.Cnt--;
	    } else {
    		if( ObjUnk13(stk, Item) == -1 ) return -1;
    		ObjLightItem( Critter->Container.Box.Box[ i ].obj, NULL );
    		Critter->Container.Box.Box[ i ].Quantity -= QuantityMax;
    		if( ItemGetObjType( Item ) == PR_ITEM_AMMO ){
        	    ItemSetCharges( Critter->Critter.Box.Box[ i ].obj, ItemGetPackQuantity( Item ) );
    		}
	    }
	    if( (Item->Pid == 54 || Item->Pid == 210 ) && (Item == LHandObj || Item == RHandObj) ){
    		if( (Leader = ObjGetOwner( Item ) ) ) ItemStealthBoyOff( Leader, Item );
	    }
	    Item->Owner = 0;
	    Item->Flags &= ~0x7000000;
	    return 0;
        }
        if( ItemGetObjType( stk->obj ) == 1 ){
            if( !ItemUseItem( Critter->Container.Box.Box[ i ].obj, Item, QuantityMax ) ) return 0;
        }
    }
    return -1;
}

void Item11( int a1, ObjBox_t *a2 )
{
    int i;
    
    for( i = a1 + 1; i < a2->Cnt; i++ ){
        a2->Box[ i - 1 ].obj = a2->Box[ i ].obj;
        a2->Box[ i - 1 ].Quantity = a2->Box[ i ].Quantity;
    }        
    a2->Cnt--;
}

int Item12( Obj_t *a1, Obj_t *a2, Obj_t *a3, int a4, int a5 )
{
    int err;
    Obj_t *Owner;
    VidRect_t Area;

    if( ItemUseItem( a1, a3, a4 ) == -1 ) return -1;
    err = ( a5 ) ? ItemAdd( a2, a3, a4 ) : Item09( a2, a3, a4 );
    if( err ){
        if( ItemAdd( a1, a3, a4 ) ){
            Owner = ObjGetOwner( a1 );
            if( !Owner ) Owner = a1;
            if( Owner->GridId != -1 ){
                ObjUnk14( a3, Owner->GridId, Owner->Elevation, &Area );
                TileUpdateArea( &Area, Owner->Elevation );
            }
        }
        return -1;
    } else {
        a3->Owner = a2;
    }
    return err;
}

int Item13( Obj_t *a1, Obj_t *a2, Obj_t *a3, int a4 )
{
    return Item12( a1, a2, a3, a4, 0 );
}

int Item14( Obj_t *a1, Obj_t *a2, Obj_t *a3, int a4 )
{
    return Item12( a1, a2, a3, a4, 1 );
}

int Item15( Obj_t *a1, Obj_t *a2 )
{
    while( a1->Container.Box.Cnt > 0 ) Item12( a1, a2, a1->Container.Box.Box->obj, a1->Container.Box.Box->Quantity, 1 );
    return 0;
}

int Item16( Obj_t *a1, Obj_t *a2 )
{
    int i, k;
    Obj_t *obj;
    Proto_t *proto;

    for( i = 0; i < a1->Container.Box.Cnt; ){
        obj = a1->Container.Box.Box[ i ].obj;
        if( OBJTYPE( obj->Pid ) )
            k = 1;
        else
            k = ProtoGetObj( obj->Pid, &proto ) == -1 || (proto->FlgExt & 0x8000000) == 0;
        if( k ){
            i++;
        } else {
            Item12( a1, a2, a1->Container.Box.Box[ i ].obj, a1->Container.Box.Box[ i ].Quantity, 1 );
        }
    }
    return 0;
}

int Item17( Obj_t *a1 )
{
    Obj_t *obj;
    Proto_t *proto;
    int i, k;

    for( i = 0; i < a1->Critter.Box.Cnt; i++ ){
        obj = a1->Critter.Box.Box[ i ].obj;
        if( OBJTYPE( obj->Pid ) )
            k = 1;
        else
            k = ProtoGetObj( obj->Pid, &proto ) == -1 || ( proto->FlgExt & 0x8000000 ) == 0;
        if( !k ){
            ItemUseItem( a1, obj->Critter.Box.Box[ i ].obj, 1 );
            UseUnk06( obj->Critter.Box.Box[ i ].obj );
        } else {
    	    i++;
	}
    }
    return 0;
}

int Item18( Obj_t *item, int bb )
{
    ObjStack_t *Box;
    Obj_t *obj;
    VidRect_t Area;
    Proto_t *proto;
    int Cnt,flg,img,Quantity,i;

    img = item->ImgId & 0xFFF;
    flg = 0;
    for( Cnt = item->Critter.Box.Cnt; Cnt; Cnt-- ){
        Box = item->Container.Box.Box;
        obj = Box->obj;
        if( Box->obj->Pid != PID_MONEY ){
	    if( obj->Flags & 0x07000000 ){
    		flg = 1;
    		if( obj->Flags & 0x04000000 ){
        	    proto = NULL;
        	    if( ProtoGetObj( item->Pid, &proto ) == -1 ) return -1;
        	    img = proto->ImgId & 0xFFF;
        	    InvUpdateStatistics( item, obj, 0 );
    		}
	    }
	    for( i = 0; i < item->Container.Box.Box->Quantity; i++ ){
    		if( ItemUseItem( item, item->Container.Box.Box->obj, 1 ) ) return -1;
    		if( ObjUnk14( item->Container.Box.Box->obj, bb, item->Elevation, 0 ) ){
		    if( ItemAdd( item, item->Container.Box.Box->obj, 1 ) ) UseUnk06( item->Container.Box.Box->obj );
		    return -1;
    		}        
	    }
        } else {
    	    Quantity = Box->Quantity;
    	    if( ItemUseItem( item, Box->obj, Quantity ) ) return -1;
    	    if( ObjUnk14( obj, bb, item->Elevation, 0 ) ){
        	if( ItemAdd( item, obj, 1 ) ) UseUnk06( obj );
        	return -1;
    	    }
    	    if( obj->Pid == PID_MONEY ) obj->Critter.State.Reaction = Quantity;
        }
    }
    if( flg ){
        ObjSetShape( item, ArtMakeId( 1, img, (item->ImgId & 0xFF0000u) >> 16, 0, (item->ImgId & 0x70000000) >> 28 ), &Area );
        if( !( (item->ImgId & 0xFF0000) >> 16 ) ) TileUpdateArea( &Area, gCurrentMapLvl );
    }
    return 0;
}

int ItemStack( Obj_t *Container, Obj_t *Item )
{
    Proto_t *proto;
    char *a, *b;
    int i = 0, charges;

    if( Item->Pid != Container->Pid ) return 0;
    if( Container->ScrId != Item->ScrId ) return 0;
    if( Container->Flags & 0x7002000 ) return 0;
    if( Item->Flags & 0x7002000 ) return 0;
    ProtoGetObj( Container->Pid, &proto );
    if( proto->Critt.Type == 1 ) return 0;
    if( Container->Container.Box.Cnt > 0 || Item->Container.Box.Cnt > 0 ) return 0;
    if( proto->Critt.Type == PR_ITEM_AMMO || Container->Pid == PID_MONEY ){
        charges = Item->Container.Charges;
        Item->Container.Charges = Container->Container.Charges;
    }

    a = (char *)&Container->Container;
    b = (char *)&Item->Container;    
    for( i = 0; i < sizeof( ObjContainer_t ); i++, a++, b++ ){
        if( *a != *b ) break;
    }
    if( proto->Critt.Type == PR_ITEM_AMMO || Container->Pid == PID_MONEY ) Item->Container.Charges = charges;

    return i >= 32;
}

char *ItemGetName( Obj_t *Obj )
{
    gItemUnk02 = ProtoGetObjName( Obj->Pid );
    return gItemUnk02;
}

char *ItemGetDsc( Obj_t *obj )
{
    return ProtoGetDudeDsc( obj->Pid );
}

int ItemGetObjType( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return 5;
    if( (obj->Pid >> 24) ) return 5;
    if( obj->Pid == 383 ) return 5;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.Type;
}

int Item23( Obj_t *obj )
{
    Proto_t *proto;

    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 18 ];
}

int Item24( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return 0;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 19 ];
}

int ItemGetItemWeight( Obj_t *item )
{
    int weight, n, charges;
    Proto_t *proto1, *proto2, *pObj;

    if( !item ) return 0;    
    ProtoGetObj( item->Pid, &proto1 );
    weight = proto1->Critt.BaseStat[ 20 ];
    if( OBJTYPE( item->Pid ) )
        n = 1;
    else
        n = ProtoGetObj( item->Pid, &proto2 ) == -1 || ( proto2->FlgExt & 0x8000000 ) == 0;
    if( !n ) weight = 0;
    switch( proto1->Critt.Type ){
        case 1:
            weight += ItemGetBackPackWeight( item );
            break;
        case 3:
            ProtoGetObj( item->Pid, &pObj );
            charges = item->Container.Charges;
            if( charges > 0 ){
                n = ItemGetObjType(item) == 3 ? item->Container.AmmoId : -1;
                if( n != -1 && ProtoGetObj( n, &proto1) != -1 ) return (proto1->Critt.BaseStat[20] * ((charges - 1) / proto1->Critt.BaseStat[1] + 1) + weight);
            }
            break;
        case 0:
            if( proto1->Pid == 3 || proto1->Pid == 232 || proto1->Pid == 348 || proto1->Pid == 349 ) weight /= 2;
            break;
    }
    return weight;
}

int Item26( Obj_t *obj )
{
    Proto_t *proto;
    int t;

    if( !obj ) return 0;
    ProtoGetObj( obj->Pid, &proto );
    switch( proto->Critt.Type ){
        case 1:
            return Item27( obj ) + proto->Critt.BaseStat[ 21 ];
        case 3:
            ProtoGetObj( obj->Pid, &proto );
            if( obj->Container.Charges > 0 ){
                t = ItemGetObjType( obj ) == 3 ? obj->Container.AmmoId : -1;
                if( t != -1 && ProtoGetObj( t, &proto ) != -1 ) 
            	    return proto->Critt.BaseStat[ 21 ] + obj->Container.Charges * proto->Critt.BaseStat[ 21 ] / proto->Critt.BaseStat[ 1 ];
            }
            break;
        case 4:
            ProtoGetObj( obj->Pid, &proto );
            return (obj->Critter.State.Reaction * obj->Container.Charges) / (( proto->Critt.Type == 4 ) ? proto->Critt.BaseStat[ 1 ] : proto->Critt.BaseStat[ 15 ]);
    }
    return proto->Critt.BaseStat[ 21 ];
}

int Item27( Obj_t *obj )
{
    int n, i;
    Obj_t *RHandObj, *LHandObj, *ArmorObj;
    Proto_t *proto;

    if( !obj ) return 0;
    n = 0;    
    if( obj->Critter.Box.Cnt > 0 ){
        for( i = 0; i < obj->Critter.Box.Cnt; i++ ){
            if( ItemGetObjType( obj->Container.Box.Box[i].obj) == 4 ){
                ProtoGetObj( obj->Container.Box.Box[ i ].obj->Pid, &proto );
                n += Item26( obj->Container.Box.Box[ i ].obj ) + ( obj->Container.Box.Box[ i ].Quantity - 1 ) * proto->Critt.BaseStat[21];
            } else {
                n += obj->Container.Box.Box[ i ].Quantity * Item26( obj->Container.Box.Box[ i ].obj );
            }
        }
    }
    if( OBJTYPE( obj->ImgId ) == 1 ){
        RHandObj = InvGetRHandObj(obj);
        if( RHandObj && (RHandObj->Flags & 0x2000000) == 0 ) n += Item26( RHandObj );
        LHandObj = InvGetLHandObj( obj );
        if( LHandObj && RHandObj != LHandObj && (LHandObj->Flags & 0x1000000) == 0 ) n += Item26( LHandObj );
        if( (ArmorObj = InvGetArmorObj( obj )) ){
            if( (ArmorObj->Flags & 0x4000000) == 0 ) n += Item26( ArmorObj );
        }
    }
    return n;
}

int ItemGetBackPackWeight( Obj_t *obj )
{
    Obj_t *LhItem, *RhItem, *Armor;
    int Weight, i;

    if( !obj ) return 0;    
    Weight = 0;
    // backpack
    for( i = 0; i < obj->Container.Box.Cnt; i++ ){
        Weight += obj->Container.Box.Box[ i ].Quantity * ItemGetItemWeight( obj->Container.Box.Box[ i ].obj );
    }
    // equipped items
    if( OBJTYPE( obj->ImgId ) == TYPE_CRIT ){
	// + right hand weight
        RhItem = InvGetRHandObj( obj );
        if( RhItem && !(RhItem->Flags & PRFLG_WORN_LHAND ) ) Weight += ItemGetItemWeight( RhItem );
	// + left hand weight
        LhItem = InvGetLHandObj( obj );
        if( LhItem && (RhItem != LhItem) && !( LhItem->Flags & PRFLG_WORN_RHAND ) ) Weight += ItemGetItemWeight( LhItem );
        // + armor weight
        Armor = InvGetArmorObj( obj );
        if( Armor && !(Armor->Flags & PRFLG_WORN_ARMOR ) ) Weight += ItemGetItemWeight( Armor );
    }
    return Weight;
}

int Item29( Obj_t *obj )
{
    int tmp;
    Proto_t *proto;

    if( !obj ) return 0;    
    if( ItemGetObjType( obj ) != PR_ITEM_WEAPON ) return 0;
    if( (gObjDude->Container.Flags & 0x10) != 0 && (gObjDude->Container.Flags & 0x20) != 0 ) return 1;
    tmp = 1;
    if( obj ){
        ProtoGetObj( obj->Pid, &proto );
        tmp = ( proto->FlgExt & 0x200 ) == 0;
    }
    if( tmp ) return 0;
    return ( ( gObjDude->Container.AmmoId & 0x10 ) || ( gObjDude->Container.AmmoId & 0x20 ) );
}

int ItemGetArtId( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return -1;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 22 ];
}

Obj_t *ItemGetSlotItem( Obj_t *obj, int a2 )
{
    Obj_t *v2;

    v2 = 0;
    if( !obj ) return 0;    
    switch( a2 ){
        case 0: case 1: case 6:
            v2 = InvGetLHandObj( obj );
            break;
        case 2: case 3: case 7:
            v2 = InvGetRHandObj( obj );
            break;
        default:
            return v2;
    }
    return v2;
}

int ItemGetSlotApCost( Obj_t *obj, int Slot, int a3 )
{
    Obj_t *v4;

    if( !obj ) return 0;
    v4 = 0;
    switch( Slot ){
        case 0: case 1: case 6:
            v4 = InvGetLHandObj( obj );
            break;
        case 2: case 3: case 7:
            v4 = InvGetRHandObj( obj );
            break;
        default:
            break;
    }
    if( v4 && ItemGetObjType( v4 ) != PR_ITEM_WEAPON ) return 2;
    return ItemGetAPCost( obj, Slot, a3 );
}

int Item33( Obj_t *obj, Obj_t *a2 )
{
    int i,v5;

    v5 = 0;
    for( i = 0; i < obj->Critter.Box.Cnt; i++ ){
        if( a2 == obj->Critter.Box.Box[ i ].obj ){
            v5 = obj->Critter.Box.Box[ i ].Quantity;
        } else if( ItemGetObjType( obj->Critter.Box.Box[ i ].obj ) == 1 ){
            v5 = Item33( obj->Critter.Box.Box[ i ].obj, a2 );
            if( v5 > 0 ) return v5;
        }        
    }        
    return v5;
}

int Item34( Obj_t *obj )
{
    Obj_t *v3;
    int i;

    if( !obj ) return 0;    
    if( (obj->Flags & 0x2000) != 0 ) return 1;
    for( i = 0; i < obj->Container.Box.Cnt; i++ ){
        v3 = obj->Container.Box.Box[ i ].obj;
        if( (v3->Flags & 0x2000) != 0 ) return 1;
        if( ItemGetObjType( v3 ) == 1 && Item34( obj->Container.Box.Box[ i ].obj ) ) return 1;
    }
    return 0;
}

Obj_t *Item35( Obj_t *obj, Obj_t *a2, int a3 )
{
    int i;
    Obj_t *p;

    if( !obj || !a2 ) return 0;        
    for( i = 0; i < obj->Container.Box.Cnt; i++ ){
        if( ItemStack( obj->Container.Box.Box[ i ].obj, a2 ) && !ItemUseItem( obj, obj->Container.Box.Box[ i ].obj, 1 ) ){
            a2->Flags |= a3;
            if( !ItemAdd( obj, a2, 1 ) ) return a2;
            a2->Flags &= ~a3;
            if( ItemAdd( obj, a2, 1 ) ) UseUnk06( a2 );
        }
        if( ItemGetObjType( obj->Container.Box.Box[ i ].obj ) == 1 ){
            p = Item35( obj->Container.Box.Box[ i ].obj, a2, a3 );
            if( p ) return p;
        }
    }        
    return 0;
}

int Item36( Obj_t *obj )
{
    Proto_t *proto;

    if( OBJTYPE( obj->Pid ) ) return 0;
    if( ProtoGetObj(obj->Pid, &proto) == -1 ) return 0;
    return proto->FlgExt & 0x8000000;
}

int ItemGetClass( Obj_t *item, int slot )
{
    unsigned int idx;
    Proto_t *proto;

    if( !item ) return 1;
    ProtoGetObj( item->Pid, &proto );
    if( slot && slot != 2 ) 
	idx = ( proto->FlgExt & 0xF0 ) >> 4;
    else
	idx = proto->FlgExt & 0x0F;
    return gItemClass[ idx ];
}

int ItemGetSkill( Obj_t *obj, int a2 )
{
    unsigned int v5;
    int v6;
    int v7;
    Proto_t *proto;

    if( !obj ) return 3;
    ProtoGetObj( obj->Pid, &proto );
    if( a2 && a2 != 2 )
        v5 = (proto->FlgExt & 0xF0u) >> 4;
    else
        v5 = proto->FlgExt & 0xF;
    v6 = gItemUnk03[ v5 ];
    if( !v6 ){
        v7 = ItemGetWeaponBase(0, obj);
        if( v7 == 1 || v7 == 3 || v7 == 4 ) return 2;
        if( (proto->FlgExt & 0x100) != 0 ) return 1;
    }
    return v6;
}

int ItemGetHitChance( Obj_t *obj, int a2 )
{
    Obj_t *v2, *v3;
    int v5;

    v2 = obj;
    if( !obj ) return 0;
    v3 = 0;
    switch( a2 ){
        case 0: case 1: case 6:
            v3 = InvGetLHandObj( obj );
            break;
        case 2: case 3: case 7:
            v3 = InvGetRHandObj( obj );
            break;
        default:
            break;
    }
    if( v3 )
        v5 = ItemGetSkill( v3, a2 );
    else
        v5 = 3;
    return SkillGetTotal( v2, v5 );    
}

int ItemGetWeaponDmg( Obj_t *Weapon, int *pMin, int *pMax )
{
    Proto_t *proto;

    if( !Weapon ) return -1;
    ProtoGetObj(Weapon->Pid, &proto);
    if( pMin ) *pMin = proto->Critt.BaseStat[ 1 ];
    if( pMax ) *pMax = proto->Critt.BaseStat[ 2 ];
    return 0;
}

int Item41( Obj_t *obj, int a2 )
{
    int v5,Val,Class,v11,v12;
    Obj_t *item;
    Proto_t *proto;

    item = NULL;
    v5 = 0;
    Val = 0;
    v11 = 0;
    v12 = 0;
    if( !obj ) return 0;
    switch( a2 ){
        case 0: case 1: case 6:
            item = InvGetLHandObj( obj );
            break;
        case 2: case 3: case 7:
            item = InvGetRHandObj( obj );
            break;
        default:
            break;
    }
    if( item ){
        ProtoGetObj( item->Pid, &proto );
        v12 = proto->Critt.BaseStat[ 1 ];
        v11 = proto->Critt.BaseStat[ 2 ];
        Class = ItemGetClass( item, a2 );
        if( Class == 2 || Class == 1 ) Val = FeatGetVal( obj, FEAT_MELEE );
    } else {
        v12 = 1;
        v11 = FeatGetVal( obj, FEAT_MELEE ) + 2;
        switch( a2 ){
            case 8: case 11:
                v5 = 3;
                break;
            case 9: case 14:
                v5 = 5;
                break;
            case 10: case 12: case 15: case 17:
                v5 = 7;
                break;
            case 13:
                v5 = 10;
                break;
            case 16: case 18:
                v5 = 9;
                break;
            case 19:
                v5 = 12;
                break;
            default:
                return RandMinMax(v5 + v12, v5 + Val + v11);
        }
    }
    return RandMinMax(v5 + v12, v5 + Val + v11);
}

int ItemGetWeaponBase( Obj_t *obj1, Obj_t *obj2 )
{
    Proto_t *proto;

    if( obj2 ){
        ProtoGetObj( obj2->Pid, &proto );
        return proto->Critt.BaseStat[ 3 ];
    }
    if( obj1 ) return CritterUnk03( obj1 );
    return 0;
}

int ItemWeaponTwoHanded( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return 0;
    ProtoGetObj( obj->Pid, &proto );
    return proto->FlgExt & 0x200;
}

int Item44( Obj_t *obj, int a2 )
{
    Obj_t *v2;

    v2 = NULL;
    if( obj ){
        switch( a2 ){
            case 0: case 1: case 6:
                v2 = InvGetLHandObj( obj );
                break;
            case 2: case 3: case 7:
                v2 = InvGetRHandObj( obj );
                break;
            default:
                break;
        }
        obj = v2;
    }
    return Item45( obj, a2 );
}

int Item45( Obj_t *obj, int a2 )
{
    unsigned int v4;
    Proto_t *proto;

    if( a2 == 5 || ( a2 >= 14 && a2 <= 19 ) ) return 17;
    if( !obj ) return 16;
    ProtoGetObj( obj->Pid, &proto );
    if( a2 && a2 != 2 ) 
	v4 = (proto->FlgExt & 0xF0) >> 4;
    else
        v4 = proto->FlgExt & 0xF;
    return gItemUnk678[ v4 ];
}

int ItemGetPackQuantity( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return 0;
    ProtoGetObj( obj->Pid, &proto );
    if( proto->Critt.Type == PR_ITEM_AMMO ) return proto->Critt.BaseStat[ 1 ];
    return proto->Critt.BaseStat[ 15 ];
}

int ItemGetAmmo( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return 0;
    ProtoGetObj( obj->Pid, &proto );
    return obj->Container.Charges;
}

int Item48( Obj_t *obj )
{
    Proto_t *v4;
    Proto_t *proto;
    
    if( !obj ) return 0;
    ProtoGetObj(obj->Pid, &proto);
    if( proto->Critt.Type != 4 ){
        ProtoGetObj( obj->Pid, &v4 );
        if( ProtoGetObj( obj->Container.AmmoId, &v4 ) == -1 ) return 0;
        proto = v4;
    }
    return proto->Critt.BaseStat[ 0 ];
}

void ItemSetCharges( Obj_t *Obj, int Charges )
{
    Proto_t *proto;

    if( !Obj ) return;    
    ProtoGetObj( Obj->Pid, &proto );
    Obj->Container.Charges = Charges;
}

int Item50( Obj_t *Obj1, Obj_t *Obj2 )
{
    Obj_t *p;
    Proto_t *proto;
    int n, PackQuantity;

    if( Obj2 ) ProtoGetObj( Obj2->Pid, &proto );
    PackQuantity = ItemGetPackQuantity( Obj2 );
    if( PackQuantity == 0 ) return -1;
    if( Obj2->Pid == PID_SOLARSCORCHER ){
	if( Item51( Obj2, 0 ) ) return -1;
	return 0;
    }
    n = -1;
    while( ( p = InvSearchObjByType( Obj1, 4, &n ) ) ){
        if( Obj2->Container.AmmoId == p->Pid && ItemUnk01(Obj2, p) ){
            if( Item51( Obj2, p ) ) return -1;
            UseUnk06( p );
            return 0;
        }
    }
    n = -1;
    while( ( p = InvSearchObjByType( Obj1, 4, &n ) ) ){        
        if( ItemUnk01( Obj2, p ) ){
            if( Item51( Obj2, p ) ) return -1;
            UseUnk06( p );
            return 0;            
        }
    }
    return -1;    
}

int ItemUnk01( Obj_t *obj1, Obj_t *obj2 )
{
    MsgLine_t msg;
    Proto_t *proto;
    Proto_t *pObj;

    if( obj1->Pid == 390 ){
        if( ItemMapGetLight() > 0xF333 ) return 1;
        IfcMsgOut( MessageGetMessage( &gItemMsg, &msg, 500 ) ); // There is not enough light to recharge this item
    } else if( obj2 ){
        ProtoGetObj(obj1->Pid, &proto);
        ProtoGetObj(obj2->Pid, &pObj);
        if( proto->Critt.Type == 3 && 
    	    pObj->Critt.Type == 4 && 
    	    pObj->Critt.BaseStat[ 0 ] == proto->Critt.BaseStat[ 13 ] && 
    	    ( !ItemGetAmmo( obj1 ) || obj1->Container.AmmoId == obj2->Pid ) 
    	) return 1;
    }
    return 0;
}

int Item51( Obj_t *obj1, Obj_t *obj2 )
{
    int charges,v6,v7,v8,v9,v10,v11,v12;
    Proto_t *v13,*proto,*pObj;

    if( !ItemUnk01( obj1, obj2 ) ) return -1;
    if( obj1 ){
        ProtoGetObj( obj1->Pid, &v13 );
        charges = obj1->Container.Charges;
    } else {
        charges = 0;
    }
    v6 = charges;
    if( obj1 ){
        ProtoGetObj(obj1->Pid, &proto);
        v7 = ( proto->Critt.Type == 4 ) ? proto->Critt.BaseStat[1] : proto->Critt.BaseStat[15];
    } else {
        v7 = 0;
    }
    v8 = v7;
    if( obj1->Pid == 390 ){
        ItemSetCharges( obj1, v7 ); 
        return 0;
    }
    
    if( obj2 ){
        ProtoGetObj( obj2->Pid, &pObj );
        v9 = obj2->Container.Charges;
    } else {
        v9 = 0;
    }
    v10 = v9;
    if( v6 < v8 ){
        if( v6 + v9 > v8 ){
            v12 = v9 - (v8 - v6);
            v11 = v8;
            v10 = v12;
        } else {
            v11 = v6 + v9;
            v10 = 0;
        }
        obj1->Container.AmmoId = obj2->Pid;
        ItemSetCharges( obj2, v10 );
        ItemSetCharges( obj1, v11 );
    }
    return v10;
}

int ItemGetRange( Obj_t *Obj, int Slot )
{
    int MaxCap,Val;
    Proto_t *proto;
    Obj_t *p;
    
    p = Obj;
    if( Obj ){
        switch( Slot ){
            case 0: case 1: case 6:
                Obj = InvGetLHandObj( Obj );
                break;
            case 2: case 3: case 7:
                Obj = InvGetRHandObj( Obj );
                break;
            default:
        	Obj = NULL;
                break;
        }
    }
    if( !Obj || Slot == 4 || Slot == 5 || ( Slot >= 8 && Slot <= 19 ) ){
        if( CritterGetInjure( p->Pid, 0x2000 ) ) return 2;
        return 1;
    }
    ProtoGetObj(Obj->Pid, &proto);
    MaxCap = ( Slot && Slot != 2 ) ? proto->Critt.BaseStat[ 5 ] : proto->Critt.BaseStat[ 4 ];
    if( ItemGetClass( Obj, Slot ) == 3 ){
        if( p == gObjDude ){
            Val = 2 * PerkLvl( gObjDude, PERK_HEAVE_HO ) + FeatGetVal( p, FEAT_STAMINA ); // +2 str per rank when determining throw weapon range for perk
        } else {
            Val = FeatGetVal( p, FEAT_STAMINA );
        }
        if( 3 * Val < MaxCap ) return 3 * Val;
    }
    return MaxCap;    
}

int ItemGetAPCost( Obj_t *obj, int HandSlot, int a3 )
{
    Obj_t *v6;
    int ApCost, Class;
    Proto_t *v14,*proto;

    v6 = NULL;
    if( obj ){
        switch( HandSlot ){
            case 0: case 1: case 6:
                v6 = InvGetLHandObj( obj );
                break;
            case 2: case 3: case 7:
                v6 = InvGetRHandObj( obj );
                break;
            default:
                break;
        }
    }
    if( HandSlot == 6 || HandSlot == 7 ) {
        if( v6 ){
    	    ProtoGetObj( v6->Pid, &v14 );
    	    if( v14->Critt.BaseStat[ 11 ] == 65 ) return 1;
        }
        if( v6->Pid == 390 ) return 0;
        return 2;        
    }
    switch( HandSlot ){
        case 14: case 15: case 16:
            ApCost = 4;
            break;
        case 11:
            ApCost = 3;
            break;
        case 12:
            ApCost = 6;
            break;
        case 17: case 18:
            ApCost = 7;
            break;
        case 13:
            ApCost = 8;
            break;
        case 19:
            ApCost = 9;
            break;
        default:
            if( !v6 || HandSlot == 4 || HandSlot == 5 || HandSlot == 8 || HandSlot == 9 || HandSlot == 10 ){
                ApCost = 3;
            } else {
                if( HandSlot && HandSlot != 2 ){
                    ProtoGetObj( v6->Pid, &proto );
                    ApCost = proto->Critt.BaseStat[ 9 ];
                } else {
                    ProtoGetObj(v6->Pid, &proto);
                    ApCost = proto->Critt.BaseStat[8];
                }
                if( obj == gObjDude && TraitSpecActive( 7 ) && ItemGetRange( obj, HandSlot ) > 2 ) ApCost--;
            }
            break;
    }
    if( obj == gObjDude ){
        Class = ItemGetClass(v6, HandSlot);
        if( PerkLvl( gObjDude, 1 ) && (Class == 2 || Class == 1) ) ApCost--;
        if( PerkLvl( gObjDude, 5 ) && Class == 4 ) ApCost--;
    }
    if( a3 ) ApCost++;
    if( ApCost < 1 ) return 1;
    return ApCost;
}

int Item54( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return -1;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 7 ];
}

int Item55( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return -1;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 10 ];
}

int Item56( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return -1;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 11 ];
}

int Item57( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return -1;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 12 ];
}

int Item58( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return -1;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 0 ];
}

int Item59( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return -1;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 6 ];
}

int ItemGetAmmoId( Obj_t *obj )
{
    if( obj && ItemGetObjType( obj ) == 3 ) return obj->Container.AmmoId;
    return -1;
}

int Item61( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return 0;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 16 ];
}

int Item62( Obj_t *obj, int a2 )
{
    int v7, v12;
    Obj_t *v10;

    if( obj == gObjDude && TraitSpecActive( 7 ) ) return 0;
    v7 = Item45( ItemGetSlotItem( obj, a2 ), a2 );
    if( v7 == PID_BAG1 || v7 == PID_FIRSTAIDKIT) return 0;
    v10 = NULL;
    if( obj ){
        switch( a2 ){
            case 0: case 1: case 6:
                v10 = InvGetLHandObj( obj );
                break;
            case 2: case 3: case 7:
                v10 = InvGetRHandObj( obj );
                break;
            default:
                break;
        }
    }
    v12 = ItemGetWeaponBase( obj, v10 );
    return v12 != 6 && v12 != 2 && v12 != 5 && ( v12 != 3 || v7 != 18 );
}

int ItemCharged( Obj_t *obj )
{
    int n, Charges;
    Proto_t *proto;

    if( !obj || ItemGetObjType( obj ) != 3 ) return 0;
    if( obj ){
        ProtoGetObj( obj->Pid, &proto );
        n = proto->Critt.Type == PR_ITEM_AMMO ? proto->Critt.BaseStat[ 1 ] : proto->Critt.BaseStat[ 15 ];
    } else {
        n = 0;
    }
    if( n <= 0 ) return 0;
    if( obj ){
        ProtoGetObj( obj->Pid, &proto );
        Charges = obj->Container.Charges;
    } else {
        Charges = 0;
    }
    return Charges > 0 && obj->Pid != 390 && ItemGetAmmoId( obj ) != -1;
}

Obj_t *ItemUnload( Obj_t *Weapon )
{
    int AmmoId, charges, n;
    Obj_t *NewObj;
    Proto_t *proto, *pObj;

    if( !ItemCharged( Weapon ) ) return NULL;
    AmmoId = ( Weapon && ItemGetObjType( Weapon ) == PR_ITEM_WEAPON ) ? Weapon->Container.AmmoId : -1;
    if( AmmoId == -1 ) return NULL;
    if( ObjCopy( &NewObj, AmmoId ) ) return NULL;
    ObjLightItem( NewObj, NULL );
    if( Weapon ){
        ProtoGetObj( Weapon->Pid, &proto );
        charges = Weapon->Container.Charges;
    } else {
        charges = 0;
    }
    n = 0;
    if( NewObj ){
        ProtoGetObj( NewObj->Pid, &pObj );
        n = ( pObj->Critt.Type == PR_ITEM_AMMO ) ? pObj->Critt.BaseStat[1] : pObj->Critt.BaseStat[15];
    }
    if( charges > n ){
        ItemSetCharges( NewObj, n );
        n = charges - n;
    } else {
        ItemSetCharges( NewObj, charges );
        n = 0;
    }
    ItemSetCharges( Weapon, n );
    return NewObj;
}

int Item65( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return -1;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 8 ];
}

int Item66( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return -1;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 9 ];
}

int Item67( Obj_t *obj, int *n )
{
    if( !n ) return -1;
    if( !obj ) return 0;
    if( obj->Pid == PID_SUPERCATTLEPROD ){
        *n *= 2;
    } else if( obj->Pid == PID_MEGAPOWERFIST ){
        *n *= 2;
    }
    return 0;
}

int Item68( Obj_t *obj )
{
    int n;

    n = ItemGetWeaponBase( 0, obj );
    return n == 6 || n == 3 || n == 5;
}

int Item69( Obj_t *obj, int a2 )
{
    int Class, base;

    Class = ItemGetClass( obj, a2 );
    Item45( obj, a2 );
    base = ItemGetWeaponBase( 0, obj );
    if( Class == ITEM_CLASS_RANGED ){
        if( a2 == PID_THROWINGKNIFE && base == 6 ) return 3;
    } else if( Class == 3 ){
        if( base == 6 || base == 3 || base == 5 ) return 2;
    }
    return 0;
}

int Item70()
{
    return 2;
}

int Item71()
{
    return 3;
}

int Item72( Obj_t *obj )
{
    int AmmoId;
    Proto_t *proto;

    proto = 0;
    if( !obj ) return 0;    
    if( ItemGetObjType( obj ) == 3 )
        AmmoId = obj->Container.AmmoId;
    else
        AmmoId = -1;
    if( AmmoId == -1 ) return 0;
    if( ProtoGetObj( AmmoId, &proto ) == -1 ) return 0;
    return proto->Critt.BaseStat[ 2 ];
}

int Item73( Obj_t *obj )
{
    int AmmoId;
    Proto_t *proto;

    proto = 0;
    if( !obj ) return 0;    
    if( ItemGetObjType( obj ) == 3 )
        AmmoId = obj->Container.AmmoId;
    else
        AmmoId = -1;
    if( AmmoId == -1 ) return 0;        
    if( ProtoGetObj( AmmoId, &proto ) == -1 ) return 0;        
    return proto->Critt.BaseStat[ 3 ];
}

int Item74( Obj_t *obj )
{
    int AmmoId;
    Proto_t *proto;

    proto = 0;
    if( !obj ) return 1;
    if( ItemGetObjType(obj) == 3 ) 
	AmmoId = obj->Container.AmmoId;
    else
        AmmoId = -1;
    if( AmmoId == -1 ) return 1;
    if( ProtoGetObj( AmmoId, &proto ) == -1 ) return 1;
    return proto->Critt.BaseStat[ 4 ];
}

int Item75( Obj_t *obj )
{
    int AmmoId;
    Proto_t *proto = NULL;

    if( !obj ) return 1;
    if ( ItemGetObjType(obj) == 3 )
        AmmoId = obj->Container.AmmoId;
    else
        AmmoId = -1;
    if( AmmoId == -1 ) return 1;
    if( ProtoGetObj( AmmoId, &proto ) == -1 ) return 1;
    return proto->Critt.BaseStat[ 5 ];
}

int ItemGetAC( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return 0;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 0 ];
}

int ItemGetDmgRes( Obj_t *obj, int Idx )
{
    Proto_t *proto;

    if( !obj ) return 0;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ Idx + 1 ];
}

int ItemGetDmgThr( Obj_t *obj, int idx )
{
    Proto_t *proto;

    if( !obj ) return 0;    
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ idx + 8 ];
}

int Item79( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return -1;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 15 ];
}

int Item80( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return -1;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 16 ];
}

int Item81( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return -1;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 17 ];
}

int ItemCharges( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return 0;    
    ProtoGetObj(obj->Pid, &proto);
    return proto->Critt.BaseStat[2];
}

int ItemGetCharges( Obj_t *obj )
{
    if( !obj ) return 0;
    return obj->Container.Charges;
}

int ItemUseCharges( Obj_t *obj, int Charges )
{
    Proto_t *proto;

    if( !obj ) return -1;
    if( Charges < 0 ) Charges = 0;
    ProtoGetObj( obj->Pid, &proto );
    if( Charges > proto->Critt.BaseStat[ 2 ] ) Charges = proto->Critt.BaseStat[ 2 ];
    obj->Container.Charges = Charges;
    return 0;
}

int Item85( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return 0;    
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 1 ];
}

int Item86( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return -1;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 0 ];
}

int Item87( Obj_t *obj )
{
    Proto_t *proto;

    if( !obj ) return 0;
    ProtoGetObj( obj->Pid, &proto );
    return proto->Critt.BaseStat[ 2 ] != 0;
}

int ItemRecharge( Obj_t *Obj1, Obj_t *Obj2 )
{
    int charges, v8;
    char stmp[80];
    MsgLine_t msg;

    if( Obj2->Pid != PID_GEIGER && Obj2->Pid != PID_STEALTHBOY1 ) return -1;
    if( Obj2->Pid == PID_MOTIONSENSOR1 ){
	charges = ItemGetCharges( Obj2 );
	if( charges > 0 )
    	    charges = ItemUseCharges( Obj2, charges - 1 );
	else
    	    charges = -1;
	if( charges ){
    	    msg.Id = 5; // '%s has no charges left.'
    	    if( MessageGetMsg( &gItemMsg, &msg ) == 1 ){
        	sprintf( stmp, msg.Text, ObjGetName( Obj2 ) );
    		IfcMsgOut( stmp );
    	    }
	} else{
    	    AutomapScanner( 1, 1 );
	}
	return 0;
    }
    if( Obj2->Pid != PID_GEIGERCOUNTER && Obj2->Pid != PID_STEALTHBOY ) return -1;	
    if( Item87( Obj2 ) )
        v8 = EvQeEnqueued( Obj2, 9 );
    else
        v8 = 0;
    if( v8 )
        ItemDeviceOff( Obj2 );
    else
        ItemDeviceOn( Obj2 );
    return 0;
}

int ItemMotSensCharge( Obj_t *obj )
{
    int charges, v4;

    if( obj )
        charges = obj->Container.Charges;
    else
        charges = 0;
    if( charges <= 0 ) return -1;
    if( !obj ) return -1;
    v4 = ItemCharges( obj );
    if( charges > v4 ) charges = v4;
    obj->Container.Charges = charges;
    return 0;
}

int ItemDeviceUse( Obj_t *obj )
{
    int charges;
    char stmp[80];
    MsgLine_t msg;

    charges = ItemGetCharges( obj );
    if( charges > 0 )
        charges = ItemUseCharges( obj, charges - 1 );
    else
        charges = -1;
    if( charges ){
        if( ObjGetOwner( obj ) == gObjDude ){
            msg.Id = 5; // // '%s has no charges left.'
            if( MessageGetMsg( &gItemMsg, &msg ) == 1 ){
                sprintf( stmp, msg.Text, ObjGetName( obj ) );
                IfcMsgOut( stmp );
            }
        }
        ItemDeviceOff( obj );
    } else {
        EvQeSchedule( ( obj->Pid == 54 || obj->Pid == 210 ) ? 600 : 3000, obj, 0, 9 );
    }
    return 0;
}

int Item90( Obj_t *obj )
{
    if( obj && Item87( obj ) ) return EvQeEnqueued( obj, 9 );
    return 0;
}

int ItemDeviceOn( Obj_t *obj )
{
    Obj_t *crit;
    int charges;
    char stmp[80];
    VidRect_t Rect;
    MsgLine_t msg;

    if( !( crit = ObjGetOwner( obj ) ) ){
        msg.Id = 9; // 'This item can only be used from the interface bar.'
        if( MessageGetMsg( &gItemMsg, &msg ) == 1 ) IfcMsgOut( msg.Text );
        return -1;
    }
    charges = ItemGetCharges( obj );
    if( charges > 0 )
        charges = ItemUseCharges( obj, charges - 1 );
    else
        charges = -1;
    if( charges ){
        if( crit == gObjDude ){
            msg.Id = 5; // '%s has no charges left.'
            if( MessageGetMsg( &gItemMsg, &msg ) == 1 ){
                sprintf( stmp, msg.Text, ObjGetName( obj ) );
                IfcMsgOut( stmp );
            }
        }
        return -1;
    } 
    if( obj->Pid == 54 || obj->Pid == 210 ){ // geigercounter or stealthboy
        EvQeSchedule( 600, obj, 0, 9 );
        obj->Pid = PID_STEALTHBOY; // stealth boy
        if( !(crit->Flags & 0x020000) ){
            crit->Flags |= 0x020000;
            ObjGetRadiusArea( crit, &Rect );
            TileUpdateArea( &Rect, crit->Elevation );
        }
    } else {
        EvQeSchedule( 3000, obj, 0, 9 );
        obj->Pid = PID_GEIGERCOUNTER;
    }
    if( crit == gObjDude ){
        msg.Id = 6; // ' %s is on.'
        if( MessageGetMsg( &gItemMsg, &msg ) == 1 ){
            sprintf( stmp, msg.Text, ObjGetName( obj ) );
            IfcMsgOut( stmp );
        }
        if( obj->Pid == PID_GEIGERCOUNTER ){
            msg.Id = 8; // 'You pass the Geiger counter over your body. The rem counter reads: %d'
            if( MessageGetMsg( &gItemMsg, &msg ) == 1 ){
                sprintf( stmp, msg.Text, CritterRadiated( crit ) );
                IfcMsgOut( stmp );
            }
        }
    }
    return 0;        
}

int ItemDeviceOff( Obj_t *obj )
{
    Obj_t *Critt;
    char stmp[80];
    MsgLine_t msg;

    Critt = ObjGetOwner( obj );
    EvQeDelB( obj, 9 );
    if( Critt && obj->Pid == PID_STEALTHBOY ) ItemStealthBoyOff( Critt, obj );
    obj->Pid = ( obj->Pid == PID_STEALTHBOY || obj->Pid == PID_STEALTHBOY1 ) ? PID_STEALTHBOY1 : PID_GEIGER; // StealthBoy / GeigerCounter
    if( Critt == gObjDude ) IfaceHandSlotUpdate( 0, -1, -1 );
    if( Critt == gObjDude ){
        msg.Id = 7; // '%s is off'
        if( MessageGetMsg( &gItemMsg, &msg ) == 1 ){
            sprintf( stmp, msg.Text, ObjGetName( obj ) );
            IfcMsgOut( stmp );
        }
    }
    return 0;
}

int ItemDeviceOffCb( Obj_t *obj )
{
    ItemDeviceOff( obj );
    return 1;
}

int ItemStealthBoyOn( Obj_t *Obj )
{
    VidRect_t Area;

    if( Obj->Flags & 0x200 ) return -1;
    Obj->Flags |= PRFLG_TR_GLASS;
    ObjGetRadiusArea( Obj, &Area );
    TileUpdateArea( &Area, Obj->Elevation );
    return 0;
}

int ItemStealthBoyOff( Obj_t *Obj1, Obj_t *Obj2 )
{
    Obj_t *Obj;
    VidRect_t Area;

    Obj = InvGetLHandObj( Obj1 );
    if( Obj && Obj != Obj2 && Obj->Pid == 210 ) return -1; // stealthboy in left hand
    Obj = InvGetRHandObj( Obj1 );
    if( Obj && Obj != Obj2 && Obj->Pid == 210 ) return -1; // stealthboy in right hand
    if( !(Obj1->Flags & PRFLG_TR_GLASS ) ) return -1;
    Obj1->Flags &= ~PRFLG_TR_GLASS;
    ObjGetRadiusArea( Obj1, &Area );
    TileUpdateArea( &Area, Obj1->Elevation );
    return 0;
}

int Item94( Obj_t *Obj )
{
    Proto_t *proto;

    if( !Obj ) return 0;
    ProtoGetObj( Obj->Pid, &proto );
    return proto->Critt.BaseStat[ 0 ]; // ?
}

int Item95( Obj_t *obj )
{
    int i, num;
    Obj_t *p;
    Proto_t *proto;

    if( !obj ) return 0;
    num = 0;
    for( i = 0; i < obj->Container.Box.Cnt; i++ ){            
        if( !(p = obj->Container.Box.Box[ i ].obj) ) continue;
        ProtoGetObj( p->Pid, &proto );
        num += obj->Container.Box.Box[ i ].Quantity * proto->Critt.BaseStat[ 19 ]; // ?
    }
    return num;
}

int ItemGetAcMod( Obj_t *Obj )
{
    Proto_t *proto;

    proto = NULL;
    if( !Obj ) return 0;
    if( ProtoGetObj(Obj->Pid, &proto) == -1 ) return 0;
    return proto->Critt.BaseStat[2];
}


int ItemGetDrMod( Obj_t *Obj ) // damage resistance
{
    Proto_t *proto;

    proto = NULL;
    if( !Obj ) return 0;
    if( ProtoGetObj(Obj->Pid, &proto) == -1 ) return 0;
    return proto->Critt.BaseStat[3];
}

int ItemGetDmgMinMod( Obj_t *Obj )
{
    Proto_t *proto;

    proto = NULL;
    if( !Obj ) return 0;
    if( ProtoGetObj(Obj->Pid, &proto) == -1 ) return 0;
    return proto->Critt.BaseStat[4];    
}


int ItemGetDmgMaxMod( Obj_t *Obj )
{
    Proto_t *proto;

    proto = NULL;
    if( !Obj ) return 0;
    if( ProtoGetObj(Obj->Pid, &proto) == -1 ) return 0;
    return proto->Critt.BaseStat[5];    
}

int Item100( Obj_t *Obj )
{
    if( !Obj ) return -1;
    return Obj->Critter.State.Reaction;    
}

int Item101( Obj_t *Critter, Obj_t *Obj2, int duration, int *a4, int *a5 )
{
    int i;
    ItemEv01_t *Effect;


    for( i = 0; i < 3; i++ ){
        if( a5[ i ] ) break;
    }
    if( i == 3 ) return -1;    
    if( !(Effect = Malloc( sizeof( ItemEv01_t ) )) ) return -1;

    Effect->Pid = Obj2->Pid;
    for( i = 0; i < 3; i++ ){
        Effect->i02[ i ] = a4[ i ];
        Effect->i03[ i ] = a5[ i ];
    }
    duration *= 600;
    if( ( Critter == gObjDude ) && TraitSpecActive( TRAIT_CHEM_RESISTANT ) ) duration /= 2;
    if( EvQeSchedule( duration, Critter, Effect, 0 ) != -1 ){ Free( Effect ); return -1; }
    return 0;    
}

void ItemChemUse( Obj_t *obj, int *stats, int a3, int *a4 )
{
    int i; int Boost; int Total; int v16; int v33; int v34; int Val;
    char v26[92]; char a1[92]; char v28[80];
    MsgLine_t msg; MsgLine_t v30; MsgLine_t Line;

    v34 = 0;v33 = 0;i = 0;
    if( *stats == -2 ){
        i = 1;
        v34 = 1;
    }
    for( ; i < 3; i++ ){
        if( stats[ i ] == -1 ) continue;
        if( stats[ i ] == FEAT_35 ) obj->Container.Charges &= ~4; // ?
        Boost = FeatGetBoost( obj, stats[ i ] );
        if( obj == gObjDude ) Val = FeatGetVal( gObjDude, stats[ i ] );
        if( v34 ) {
            Boost += RandMinMax( a4[i - 1], a4[ i ]);
            v34 = 0;
        } else {
            Boost += a4[ i ];
        }
        if( stats[ i ] == FEAT_35 ){
            Total = FeatGetTotal( obj, FEAT_35 );
            if( Boost + Total <= 0 && obj != gObjDude ){
                // '%s succumbs to the adverse effects of chems.'
                sprintf( a1, MessageGetMessage( &gItemMsg, &Line, 600 ), CritterGetName( obj ) );
                CombatKillPoison( obj, a1 );
            }
        }
        FeatSetBoost( obj, stats[ i ], Boost );
        if( obj != gObjDude ) continue;
        if( stats[ i ] == FEAT_35 ) IfaceRenderHP( 1 );
        v16 = FeatGetVal( obj, stats[ i ] );
        if( v16 == Val ) continue;
        msg.Id = v16 <= Val ? 2 : 1; // 'You lost %d %s.' : 'You gained %d %s.'
        if( MessageGetMsg(&gItemMsg, &msg) != 1 ) continue;
        sprintf( v28, msg.Text, Val - v16, FeatGetName( stats[ i ] ) );
        IfcMsgOut( v28 );
        v33 = 1;            
    }
    if( FeatGetVal( obj, FEAT_35 ) > 0 ){
        if( obj == gObjDude && !v33 ){
            if( a3 ){
                msg.Id = 10; // Nothing happens
                if( MessageGetMsg( &gItemMsg, &msg ) == 1 ) IfcMsgOut( msg.Text );
            }
        }
    } else if( obj == gObjDude ){
        msg.Id = 4; // you suffer a fatal heart attack from chem overdose
        if( MessageGetMsg( &gItemMsg, &msg ) == 1 ) strcpy( v26, msg.Text );
    } else {
        sprintf( v26, MessageGetMessage( &gItemMsg, &v30, 600 ), CritterGetName( obj ) );
    }
}

int Item102( Obj_t *Obj, int Pid )
{
    int i, idx, *Ptr;

    for( i = 0; i < 9; i++ ){
        if( Pid == gItemGVar[ i ].Pid ) break;
    };
    if( i >= 9 ) return 1;
    idx = i;
    if( !gItemGVar[ idx ].i03 ) return 1;
    if( !(Ptr = EvQeFindPtr( Obj, 0 )) ) return 1;
    for( i = 1; i < gItemGVar[ idx ].i03; i++  ){
	if( Pid == *Ptr ) return 0;
	if( i >= gItemGVar[ idx ].i03 ) break;
        if( !EvQeFindNextPtr( Obj, 0 ) ) return 1;
    }
    return 0;
}

int ItemUnk07( Obj_t *Obj1, Obj_t *Obj2 )
{
    int *stats; int GVarId; int Pid; int v10; int v11; 
    MsgLine_t v12;
    char *Message;
    Proto_t *proto;

    if( CritterIsDead(Obj1) ) return -1;
    if( CritterGetBodyType(Obj1) == 2 ) return -1;
    ProtoGetObj(Obj2->Pid, &proto);
    stats = proto->Critt.BaseStat;
    if( Obj2->Pid == 260 && ItemGVarDefined(259) ){
        ItemWithdrawalEnd(Obj1, 70u);
        if( Obj1 == gObjDude ){
            GVarId = ItemGetGVarId(259);
            if( GVarId != -1 ) gGVals[GVarId] = 0;
            if( !ItemGVarDefined(-1) ) CritterUnk36(4);
        }
        return 0;
    }
    Pid = Obj2->Pid;
    gItemUnk98 = Obj1;
    gItemGVarId = ItemGetGVarId(Pid);
    gItemUnk99 = stats[16];
    EvQeRun( 2, (void *)Item110 );
    if( Item102( Obj1, Obj2->Pid ) ){
        ItemChemUse( Obj1, stats, 1, stats + 3 );
        Item101( Obj1, Obj2, stats[6], stats, stats + 7 );
        Item101( Obj1, Obj2, stats[10], stats, stats + 11 );
    } else if( Obj1 == gObjDude ){
        Message = MessageGetMessage( &gItemMsg, &v12, 50 ); // 'That didn't seem to do that much.'
        IfcMsgOut( Message );
    }
    if ( !ItemGVarDefined( Obj2->Pid ) ){
        v10 = stats[14];
        if( Obj1 == gObjDude ){
            if( TraitSpecActive(11) ) v10 *= 2;
            if( TraitSpecActive(12) ) v10 /= 2;
            if( PerkLvl(gObjDude, 42u) ) v10 /= 2;
        }
        if( RandMinMax(1, 100) <= v10 ){
            ItemSchedEffect(Obj1, 1, stats[16], stats[15], Obj2->Pid);
            if( Obj1 == gObjDude ){
                v11 = ItemGetGVarId(Obj2->Pid);
                if( v11 != -1 ) gGVals[ v11 ] = 1;
                CritterUnk37( 4 );
            }
        }
    }
    return 1;    
}

int Item103( Obj_t *obj, ItemEv01_t *eff )
{
    if( PartyMembRdy( obj ) ) return 0;
    Item105( obj, eff );
    return 1;
}

int Item105( Obj_t *Obj, ItemEv01_t *eff )
{
    if( !Obj ) return 0;
    if( OBJTYPE(Obj->Pid) != 1 ) return 0;
    ItemChemUse( Obj, eff->i02, 0, eff->i03 );
    return Obj->Critter.State.CombatResult < 0;
}

int ItemLoadEffects( xFile_t *fh, ItemEv01_t **a2 )
{
    ItemEv01_t *p;
    int i,tmp[3];
    
    if( !(p = Malloc( sizeof( ItemEv01_t ) )) ) return -1;
    if( dbreadBeiBlk( fh, tmp, 3 ) == -1 ){ Free( p ); return -1; }
    if( dbreadBeiBlk( fh, p->i03, 3 ) == -1 ){ Free( p ); return -1; }
    for( i = 0; i < 3; i++ ) p->i02[ i ] = tmp[ i ];
    *a2 = p;
    return 0;
}

int ItemSaveEffects( xFile_t *fh, ItemEv01_t *a2 )
{
    if( dbputBeiBlk( fh, a2->i02, 3 ) == -1 ) return -1;
    if( dbputBeiBlk( fh, a2->i03, 3 ) == -1 ) return -1;
    return 0;
}

int ItemSchedEffect( Obj_t *obj, int a2, int RecoveryTime, int EffectId, int a5 )
{
    EvEntry_t *ef;

    if( !(ef = Malloc( sizeof( EvEntry_t ) )) ) return -1;
    ef->i01 = a2;
    ef->i02 = a5;
    ef->i03 = EffectId;
    if( EvQeSchedule(600 * RecoveryTime, obj, ef, 2) != -1 ){ Free( ef ); return -1; }
    return 0;    
}

int Item109( Obj_t *obj, EvEntry_t *effect )
{
    if( PartyMembRdy( obj ) ) return 0;
    if( !effect->i01 ) ItemWithdrawalEnd( obj, effect->i03 );
    return 1;
}

int Item110( Obj_t *obj, EvEntry_t *Effect )
{
    if( obj != gItemUnk98 ) return 0;
    if( ItemGetGVarId( Effect->i02) != gItemGVarId ) return 0;
    if( !Effect->i01 ) ItemWithdrawalEnd( gItemUnk98, Effect->i03 );
    ItemSchedEffect( obj, 1, gItemUnk99, Effect->i03, Effect->i02 );
    gItemUnk98 = NULL;
    return 1;
}

int ItemWithdrawal( Obj_t *obj, EvEntry_t *effect )
{
    int GVarId;

    if( effect->i01 ){
        ItemWithdrawalStart( obj, effect->i03, effect->i02 );
    } else {
        if( effect->i03 == 70 ) return 0;
        ItemWithdrawalEnd( obj, effect->i03 );
        if( obj == gObjDude ){
            GVarId = ItemGetGVarId( effect->i02 );
            if( GVarId != -1 ) gGVals[ GVarId ] = 0;
            if( !ItemGVarDefined( -1 ) ) CritterUnk36( 4 );
        }
    }
    return obj == gObjDude;
}

int ItemWithdrawalLoad( xFile_t *fh, EvEntry_t **wd )
{
    EvEntry_t *p;
    int tmp2, tmp1;

    if( !(p = Malloc( sizeof( EvEntry_t ) )) ) return -1;
    if( dbgetBei(fh, &p->i01) == -1 || dbgetBei(fh, &tmp1) == -1 || dbgetBei(fh, &tmp2) == -1 ){ Free( p ); return -1; }
    p->i02 = tmp1;
    p->i03 = tmp2;
    *wd = p;
    return 0;
}

int ItemWithdrawalSave( xFile_t *fh, EvEntry_t *event )
{
    if( dbputBei( fh, event->i01 ) == -1 ) return -1; 
    if( dbputBei( fh, event->i02 ) == -1 ) return -1;
    if( dbputBei( fh, event->i03 ) == -1 ) return -1;
    return 0;
}

int ItemWithdrawalStart( Obj_t *Obj, int EffectId, int a3 )
{
    int RecoveryTime;

    if( OBJTYPE( Obj->Pid ) != TYPE_CRIT ) return eprintf( "\nERROR: perform_withdrawal_start: Was called on non-critter!" );
    PerkAddEffect( Obj, EffectId );
    if( Obj == gObjDude ) IfcMsgOut( PerkGetDsc( EffectId ) ); // player    
    RecoveryTime = 10080;
    if( Obj == gObjDude ){
        if( TraitSpecActive( TRAIT_CHEM_RELIANT ) ) RecoveryTime = 5040;
        if( PerkLvl( gObjDude, PERK_FLOWER_CHILD ) ) RecoveryTime /= 2; // flower child
    }
    return ItemSchedEffect( Obj, 0, RecoveryTime, EffectId, a3 );
}

void ItemWithdrawalEnd( Obj_t *Obj, int EffectId )
{
    MsgLine_t msg;

    if( OBJTYPE( Obj->Pid ) == 1 ){
        PerkRemoveEffect( Obj, EffectId );
        if( Obj == gObjDude ){
            msg.Id = 3;
            if( MessageGetMsg( &gItemMsg, &msg ) == 1 ) IfcMsgOut( msg.Text );
        }
    } else {
        eprintf( "\nERROR: perform_withdrawal_end: Was called on non-critter!" );
    }
}

int ItemGetGVarId( int Pid )
{
    int i;

    for( i = 0; i < 9; i++ ){
        if( Pid == gItemGVar[ i ].Pid ) return gItemGVar[ i ].GVarId;
    }
    return -1;
}

void ItemGVarSet( int ObjPid )
{
    int GVarId;

    GVarId = ItemGetGVarId( ObjPid );
    if( GVarId != -1 ) gGVals[ GVarId ] = 1;
    CritterUnk37(4);
}

void ItemGVarClr( int ObjPid )
{
    int id;

    id = ItemGetGVarId( ObjPid );
    if( id != -1 ) gGVals[ id ] = 0;
    if( !ItemGVarDefined( -1 ) ) CritterUnk36( 4 );
}

int ItemGVarDefined( int Pid )
{
    int i;
    
    for( i = 0; i < 9; i++ ){
        if( Pid == gItemGVar[ i ].Pid ) return gGVals[ gItemGVar[ i ].GVarId ] != 0;
	if( Pid == -1 && gGVals[ gItemGVar[ i ].GVarId ] ) return 1;
    }
    return 0;    
}

int ItemGetMoneyAll( Obj_t *obj )
{
    int Money, i, MoneyAll;
    
    Money = 0;    
    for( i = 0; i < obj->Container.Box.Cnt; i++ ){
        if( obj->Container.Box.Box[ i ].obj->Pid == PID_MONEY ){
            Money += obj->Container.Box.Box[ i ].Quantity;
        } else if( ItemGetObjType( obj->Container.Box.Box[ i ].obj ) == TYPE_CRIT ){
            MoneyAll = ItemGetMoneyAll( obj->Container.Box.Box[ i ].obj );
            Money += MoneyAll;
        }            
    }
    return Money;
}

int ItemBarter( Obj_t *obj, int Value )
{
    int Money, j, Quantity, i, MoneyAll;
    Obj_t *item;

    Money = ItemGetMoneyAll( obj );
    if( Value < 0 ){
        if( Money < -Value ) return -1;
    }
    if( Value <= 0 || Money ){        
        for( j = 0; j < obj->Critter.Box.Cnt; j++ ){
            if( !Value ) break;
            if( obj->Critter.Box.Box[ j ].obj->Pid == PID_MONEY ){
                if( Value > 0 ){
                    obj->Critter.Box.Box[ j ].Quantity += Value;
                    Value = 0;
                } else {
            	    Quantity = obj->Critter.Box.Box[ j ].Quantity;
            	    if( -Value >= Quantity ){ // remove item
                	ObjDestroy( obj->Critter.Box.Box[ j ].obj, 0 );
                	Value += Quantity;
                	for( i = j + 1; i < obj->Critter.Box.Cnt; i++ ){
                    	    obj->Critter.Box.Box[ i - 1 ].obj = obj->Critter.Box.Box[ i ].obj;
                    	    obj->Critter.Box.Box[ i - 1 ].Quantity = obj->Critter.Box.Box[ i ].Quantity;
                	}
                	j = -1;
                	obj->Critter.Box.Cnt--;
            	    }            	    
            	}
            } else {
                obj->Critter.Box.Box[ j ].Quantity += Value;
                Value = 0;                
            }
        }
        for( i = 0; i < obj->Critter.Box.Cnt; i++ ){
    	    if( Value == 0 ) return 0;
            if( ItemGetObjType( &obj->Critter.Box.Box->obj[ i ] ) == TYPE_CRIT ){
                MoneyAll = ItemGetMoneyAll( &obj->Critter.Box.Box->obj[ i ] );
                if( Value <= 0 || MoneyAll <= 0 ){
                    if( Value < 0 ){
                        if( MoneyAll < -Value ){
                            if( !ItemBarter( &obj->Critter.Box.Box->obj[ i ], MoneyAll ) ) Value += MoneyAll;
                        } else if( !ItemBarter( &obj->Critter.Box.Box->obj[ i ], Value ) ){
                            Value = 0;
                        }
                    }
                } else if( !ItemBarter( &obj->Critter.Box.Box->obj[ i ], Value ) ){
                    Value = 0;
                }
            }
        }
        return 0;
    }
    if( ObjCopy( &item, PID_MONEY ) ) return -1;
    ObjLightItem( item, 0 );
    if( ItemAdd( obj, item, PID_MONEY ) ){ ObjDestroy( item, 0 ); return -1; }
    return 0;
}

int ItemGetMoney( Obj_t *Obj )
{
    if ( Obj->Pid != PID_MONEY ) return -1;
    return Obj->Container.Charges;
}

int ItemSetMoney( Obj_t *obj, int Value )
{
    if( obj->Pid != PID_MONEY ) return -1;
    obj->Container.Charges = Value;
    return 0;
}

int ItemGridLightInit()
{
    ItemGridLightReset();
    return 0;
}

int ItemMapGetLight()
{
    return gItemLightLvl;
}

void ItemMapModifyLight( int LightModifier, int TilesUpdate )
{
    ItemMapSetLight( gItemLightLvl + LightModifier, TilesUpdate );
}

void ItemMapSetLight( int Intensity, int TilesUpdateFlg )
{
    int LightLvl;
    int PrevLightLvl;

    LightLvl = 0x3333 * PerkLvl( gObjDude, PERK_NIGHT_VISION ) + Intensity;
    if( LightLvl < 0x4000 ) LightLvl = 0x4000;
    if( LightLvl > 0x10000 ) LightLvl = 0x10000;
    PrevLightLvl = gItemLightLvl;
    gItemLightLvl = LightLvl;
    if( TilesUpdateFlg ){
        if( PrevLightLvl != LightLvl ) TileUpdate();
    }
}

int ItemGridGetLightA( int MapLvl, int GridPos )
{
    if( MapLvl > 2 || GridPos > 39999 ) return 0;
    if( gItemGridLight[ 40000 * MapLvl + GridPos ] >= 0x10000 ) return 0x10000;
    return gItemGridLight[ 40000 * MapLvl + GridPos ];
}

int ItemGridGetLight( unsigned int MapLvl, unsigned int GridPos )
{
    if ( MapLvl > 2 || GridPos > 39999 ) return 0;
    return gItemGridLight[ 40000 * MapLvl + GridPos ];
}

void ItemGridSetLight( int MapLvl, int GridPos, int Intensity )
{
    if( MapLvl > 2 && GridPos > 39999 ) return;
    gItemGridLight[40000 * MapLvl + GridPos] = Intensity;
}

void ItemGridLightInc( unsigned int MapLvl, unsigned int Pos, int Val )
{
    if( MapLvl <= 2 && Pos <= 39999 ) gItemGridLight[ 40000 * MapLvl + Pos] += Val;
}

void ItemGridLightDec( unsigned int MapLvl, unsigned int Pos, int Val )
{
    if( MapLvl <= 2 && Pos <= 39999 ) gItemGridLight[ 40000 * MapLvl + Pos] -= Val;
}

void ItemGridLightReset()
{
    int lvl, i;

    for( lvl = 0; lvl < 3; lvl++ ){
        for( i = 0; i < 200*200; i++ ){
            gItemGridLight[ lvl * 200 * 200 + i ] = 655;
        }
    }
}

