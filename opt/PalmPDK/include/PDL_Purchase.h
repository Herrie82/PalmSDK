/*
*   PDL
*   Copyright 2010-2011 Hewlett-Packard Development Company, L.P. All rights reserved.
*/

#ifndef __PDLPURCHASE__
#define __PDLPURCHASE__

#ifdef __cplusplus
extern "C" {
#endif

#include "PDL_types.h"
	
/* Returns a list of all items available to this application,
 * and relevant information for each. */
PDL_ItemCollection *PDL_GetAvailableItems(void);

/* Purchase the item with the sent-in itemID. This will bring up
 * a system-level confirmation dialog before the purchase goes through.
 * If the user has not yet set up payment for in-app purchases, the 
 * dialogs to set that up will come up first.
 * 
 * qty is the quantity of this item you wish to buy. At present, it must
 * be set to 1. Any other value will return an error. In future, this 
 * functionality will be implemented and the restriction will be removed.
 *  
 * "usr" is a string no longer than 255 bytes, which is returned
 * with the receipt of the purchase. It is a simple usr value for 
 * you to use to keep track of your purchases if you wish. You can
 * send NULL if you don't want to make use of it. */
PDL_ItemReceipt *PDL_PurchaseItem(const char *itemID, int qty, const char *usr);

/* Returns relevant information about a specific purchase item,
 * including whether or not the user has bought it already. */
PDL_ItemInfo *PDL_GetItemInfo(const char *itemID);

/* In the event of a pending purchase, you may call this to get information
 * about the status of that pending purchase. The orderNo will have been
 * given to you when you in the reply when you first purchased the item. */
PDL_ItemReceipt *PDL_GetPendingPurchaseInfo(const char *orderNo);

/* Given a PDL_ItemInfo, this returns a string containing the JSON
 * formatted values of that PDL_ItemInfo. This string is valid until
 * you free the PDL_ItemInfo. */ 
const char *PDL_GetItemJSON(PDL_ItemInfo *itemInfo);

/* Given a PDL_ItemReceipt, this returns a string containing the JSON
 * formatted values of that PDL_ItemReceipt. This string is valid until
 * you free the PDL_ItemReceipt. */ 
const char *PDL_GetItemReceiptJSON(PDL_ItemReceipt *receipt);

/* Given a PDL_ItemCollection, this returns a string containing the JSON
 * formatted values of that PDL_ItemCollection. This string is valid until
 * you free the PDL_ItemCollection. */ 
const char *PDL_GetItemCollectionJSON(PDL_ItemCollection *itemCollectionInfo);

/* These functions free PDL_ItemInfo, PDL_ItemReceipt, and PDL_FreeItemCollection
 * objects. You are responsible for freeing all instances of these that you
 * are given as return values. */
void PDL_FreeItemInfo(PDL_ItemInfo *itemInfo);
void PDL_FreeItemReceipt(PDL_ItemReceipt *itemInfo);
void PDL_FreeItemCollection(PDL_ItemCollection *itemCollectionInfo);

#ifdef __cplusplus
}
#endif

#endif
