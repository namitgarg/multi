<?php

/**
 * @file
 * Template File to Display the Result.
 */
/*
 * Informatin about list varibles.
 * $list[0] Stores the Following Data
 * $list[0]['search-word']=> Stores the search word.
 * $list[0]['replace-word']=> Stores the replace word.
 * $list[0]['opn']=> Stores operation performed ie Search,Replace,Complete.
 * $list[0]['field_result']=> Stores the Meta field you searched.
 * $list[0]['detail'] => Stores the nid and link of the Node.
 * $list[0][matches]=> Stores the the array for Metatags found.
 */
if(isset($list[0]['matches']))
 {
  $meta_field = $list[0]['field_result'];
  $title = t('Metatag Replace Result for Search');
  $type = 'ol';
  $attributes = array(
    'id' => 'metatag-replace-result-search',
    'class' => 'metatag-replace-result',
  );
  if ($meta_field != 'all') {
    $search_meta_list = $list[0]['matches'];
    $search_node_link = $list[0]['detail']['node_link'];
    foreach ($search_node_link as $keys => $values) {
      $items[] = array(
        'data' => $values,
        'children' => array(
          'data' => "Meta $meta_field : " . $search_meta_list[$keys],
        ),
      );
    }
  }
  else {
    foreach ($list[0]['matches'] as $keys => $values) {
     $items[] = array(
       'data' => $list[0]['detail'][$keys]['node_link'][0],
       'children' => $values,
     );
    }
  }
    print theme('item_list', array(
      'items' => $items,
      'title' => $title,
      'type' => $type,
      'attributes' => $attributes,
    )
    );
}
?>
