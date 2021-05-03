/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Kernel includes. */
#include <kernel/list.h>

/*******************************************************************************
**                          FUNCTION IMPLEMENTATIONS
*******************************************************************************/
BaseType_t vListIsItemValueContainedWithin(List_t *pxList, TickType_t xItemValue)
{
    ListItem_t *pxListItem;
    listFOR_EACH_LIST_ITEM(pxListItem, pxList)
    {
        if (listGET_LIST_ITEM_VALUE(pxListItem) == xItemValue)
        {
            return pdTRUE;
        }
    }
    return pdFALSE;
}

void vListSplice(List_t *pxListOwner, List_t *pxListAdd)
{
    ListItem_t *pxListItem, *pxListItemNext;
    listFOR_EACH_LIST_ITEM_SAFE(pxListItem, pxListItemNext, pxListAdd)
    {
        uxListRemove(pxListItem);
        vListInsertEnd(pxListOwner, pxListItem);
    }
}
