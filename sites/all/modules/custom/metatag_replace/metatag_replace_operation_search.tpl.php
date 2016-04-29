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
 * $list[0]['field_result']=> Stores the Meta field Selected.
 * $list[0]['nid'] => Stores the Nid of the Node.
 * $list[0][node_title']=> Stores The Node Title of the Node.
 * $list[0][matches]=> Stores the Result for individual field.
 * $list[0]['matches']['title'] => Stores the Meta Title.
 * $list[0]['matches']['description'] => Stores the Meta Description.
 * $list[0]['matches']['abstract'] => Stores the Meta Abstract.
 * $list[0]['matches']['keywords'] => Stores the Meta Keywords.
 */
print("template <pre>");
print_r($list[0]);
print("</pre>");
if(isset($list[0]['node_link']))
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
  $search_node_link = $list[0]['node_link'];
  foreach ($search_node_link as $keys => $values) {
  $items[] = array(
    'data' => $values,
    'children' => array(
        'data' => "Meta $meta_field : ".$search_meta_list[$keys],
      ),
  );
  }
  print theme('item_list', array(
    'items' => $items,
    'title' => $title,
    'type' => $type,
    'attributes' => $attributes,
  )
      );
}
else {
  $search_meta_list = $list[0]['matches']['title'];
  $search_meta_description = $list[0]['matches']['description'];
  $search_meta_abstract = $list[0]['matches']['abstract'];
  $search_meta_keywords = $list[0]['matches']['keywords'];
  $search_node_title = $list[0]['node_title'];
  $search_nid = $list[0]['nid'];
  foreach ($search_node_title as $keys => $values) {
    $children = array();
    if (!empty($search_meta_list[$keys])) {
      $children[] = "Meta Title : $search_meta_list[$keys]";
    }
    if (!empty($search_meta_description[$keys])) {
      $children[] = "Meta Description : $search_meta_description[$keys]";
    }
    if (!empty($search_meta_abstract[$keys])) {
      $children[] = "Meta Abstract : $search_meta_abstract[$keys] ";
    }
    if (!empty($search_meta_keywords[$keys])) {
      $children[] = "Meta Keywords : $search_meta_keywords[$keys]";
    }
    $items[] = array(
      'data' => l($values, 'node/' . $search_nid[$keys]),
      'id' => $search_nid[$keys],
      'children' => $children,
    );
  }
  print theme('item_list', array(
    'items' => $items,
    'title' => $title,
    'type' => $type,
    'attributes' => $attributes,
  )
      );
}
}
?>
