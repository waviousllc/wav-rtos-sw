/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _KERNEL_LIST_H_
#define _KERNEL_LIST_H_

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <list.h>

/** @brief  Gets pointer to previous list item */
#define listGET_PREV( pxListItem )  ( ( pxListItem )->pxPrevious )

/** @brief  Loops through list for each item */
#define listFOR_EACH_LIST_ITEM( pxListItem, pxList )                           \
    for (pxListItem = listGET_HEAD_ENTRY(pxList);                              \
         pxListItem != listGET_END_MARKER(pxList);                             \
         pxListItem = listGET_NEXT(pxListItem))

/** @brief  Loops through list for each item */
#define listFOR_EACH_LIST_ITEM_SAFE( pxListItem, pxListItemNext, pxList )      \
    for (pxListItem = listGET_HEAD_ENTRY(pxList),                              \
         pxListItemNext = listGET_NEXT(pxListItem);                            \
         pxListItem != listGET_END_MARKER(pxList);                             \
         pxListItem = pxListItemNext,                                          \
         pxListItemNext = listGET_NEXT(pxListItemNext))

/**
 * @brief   List Item Value Contained Within Function
 *
 * @details Checks if pxList contains a item which has a value field
 *          matching xItemValue
 *
 * @param[in]   pxList      pointer to list to check in.
 * @param[in]   xItemValue  item value to check if any ListItem_t contains.
 *
 * @return      returns whether list contains a ListItem_t with specified value.
 * @retval      pdTRUE if list contains an item with specified value.
 * @retval      pdFALSE if list does not contain an item with specified value.
 */
BaseType_t vListIsItemValueContainedWithin(List_t *pxList, TickType_t xItemValue);

/**
 * @brief   List Splice Function
 *
 * @details Splices two lists together by placing all items contained in
 *          pxListAdd at the end of pxListOwner.
 *
 * @param[in]   pxListOwner     pointer to list to be added to.
 * @param[in]   pxListAdd       pointer to list to add.
 *
 * @return      void.
 */
void vListSplice(List_t *pxListOwner, List_t *pxListAdd);

#endif /* _KERNEL_LIST_H_ */
