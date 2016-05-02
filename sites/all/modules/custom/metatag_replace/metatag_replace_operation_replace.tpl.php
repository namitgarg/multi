<?php

/**
 * @file
 * Template File to Display the Result for Search.
 */
/*
 * Informatin about list varibles.
 * $list[0] Stores the Following Data
 * $list[0]['opn']=> Stores operation state.
 * $list[0]['field_result']=> Stores the Meta field Selected.
 * $list[0]['nid'] => Stores the Nid of the Node.
 * $list[0][node_title']=> Stores The Node Title of the Node.
 * $list[0]['replaced] => Stores the Replace result for individual field.
 * $list[0]['replaced']['title'] => Stores the Meta Title.
 * $list[0]['replaced']['description']=> Stores the Meta Description.
 * $list[0]['replaced']['abstract'] => Stores the Meta Abstract.
 * $list[0]['matches']['keywords'] => Stores the Meta Keywords.
 */
if (isset($list[0]['detail'])) {
    $meta_field = $list[0]['field_result'];
    $title = t('Metatag Replace Result for Replace');
    $type = 'ol';
    $attributes = array(
      'id' => 'metatag-replace-result-replace',
      'class' => 'metatag-replace-result',
    );
    if ($meta_field != 'all') {
        $replace_meta_list = $list[0]['replace'];
        $replace_node_link = $list[0]['detail']['node_link'];
        foreach ($replace_node_link as $keys => $values) {
            $items[] = array(
              'data' => $values,
              'children' => array(
                'data' => "Meta $meta_field : $replace_meta_list[$keys]",
              ),
            );
        }
    }
    else {
        foreach ($list[0]['replace'] as $keys => $values) {
            $items[] = array(
              'data' => $list[0]['detail'][$keys]['node_link'],
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
