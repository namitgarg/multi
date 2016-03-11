<?php
/**
 * @file
 * Template File to Display the Result.
 */
/*
 * $list[0] Stores the Following Data
 * $list[0]['search-word']=> Stores the search word.
 * $list[0]['replace-word']=> Stores the replace word.
 * $list[0]['opn']=> Stores operation performed ie Search,Replace,Complete.
 * $list[0]['field_result']=> Stores the Meta field Selected.
 * $list[0]['nid'] => Stores the Nid of the Node.
 * $list[0][node_title']=> Stores The Node Title of the Node.
 * ===========For Search operation===============
 * $list[0][matches]=> Stores the Result for individual field.
 * $list[0]['matches']['title'] => Stores the Meta Title.
 * $list[0]['matches']['description'] => Stores the Meta Description.
 * $list[0]['matches']['abstract'] => Stores the Meta Abstract.
 * $list[0]['matches']['keywords'] => Stores the Meta Keywords.
 * ==========For Replace Operation================
 * $list[0]['replaced] => Stores the Replace result for individual field.
 * $list[0]['replaced']['title'] => Stores the Meta Title.
 * $list[0]['replaced']['description']=> Stores the Meta Description.
 * $list[0]['replaced']['abstract'] => Stores the Meta Abstract.
 * $list[0]['matches']['keywords'] => Stores the Meta Keywords.
 */

$prefix = "<ol class='meta-results'>";
$suffix = "</ol>";
$meta_field = $list[0]['field_result'];
print ("<h3>Search Results For Meta Tags </h3>");
print ($prefix);
// To display Results when Metya tags are searched.
if ($list[0]['opn'] == 'search' || $list[0]['opn'] == 'replace') {
  if ($meta_field != 'all') {
    $search_meta_list = $list[0]['matches'];
    $search_node_title = $list[0]['node_title'];
    $search_nid = $list[0]['nid'];
    foreach ($search_node_title as $keys => $values) {
      print ("<li>" . l($values, 'node/' . $search_nid[$keys]) . "</li>");
      print ("<small>  Meta $meta_field : </small>" . $search_meta_list[$keys]);
    }
  }
  else {
    $search_meta_list = $list[0]['matches']['title'];
    $search_meta_description = $list[0]['matches']['description'];
    $search_meta_abstract = $list[0]['matches']['abstract'];
    $search_meta_keywords = $list[0]['matches']['keywords'];
    $search_node_title = $list[0]['node_title'];
    $search_nid = $list[0]['nid'];
    foreach ($search_node_title as $keys => $values) {
      print ("<li>" . l($values, 'node/' . $search_nid[$keys]) . "</li><ul class='meta_tag_results'>");
      if (!empty($search_meta_list[$keys])) {
        print ("<li> <small>Meta Title : </small> " . $search_meta_list[$keys] . "</li>");
      }
      if (!empty($search_meta_description[$keys])) {
        print ("<li> <small>Meta Description : </small> " . $search_meta_description[$keys] . "</li>");
      }
      if (!empty($search_meta_abstract[$keys])) {
        print ("<li> <small>Meta Abstract : </small> " . $search_meta_abstract[$keys] . "</li>");
      }
      if (!empty($search_meta_keywords[$keys])) {
        print ("<li> <small>Meta Keywords : </small> " . $search_meta_keywords[$keys] . "</li>");
      }
      print ("</ul>");
    }
  }
}
// To Display Results when Meta tags are replaced.
elseif ($list[0]['opn'] == 'complete') {
  $meta_field = $list[0]['field_result'];
  if ($meta_field == 'all') {
    $replace_nid = $list[0]['nid'];
    $replace_node_title = $list[0]['node_title'];
    $replace_meta_title = $list[0]['replaced']['title'];
    $replace_meta_description = $list[0]['replaced']['description'];
    $replace_meta_abstract = $list[0]['replaced']['abstract'];
    $replace_meta_keywords = $list[0]['replaced']['keywords'];
    foreach ($replace_node_title as $keys => $values) {
      print ("<li>" . l($values, 'node/' . $replace_nid[$keys]) . "</li><ul class='meta_tag_results'>");
      if (!empty($replace_meta_title[$keys])) {
        print ("<li> <small> Meta Title : </small> " . $replace_meta_title[$keys] . "</li>");
      }
      if (!empty($replace_meta_description[$keys])) {
        print ("<li> <small> Meta Description : </small> " . $replace_meta_description[$keys] . "</li>");
      }
      if (!empty($replace_meta_abstract[$keys])) {
        print ("<li> <small> Meta Abstract : </small> " . $replace_meta_abstract[$keys] . "</li>");
      }
      if (!empty($replace_meta_keywords[$keys])) {
        print ("<li> <small> Meta Keywords : </small> " . $replace_meta_keywords[$keys] . "</li>");
      }
      print ("</ul>");
    }
  }
  else {
    $replace_meta_list = $list[0]['replaced'];
    $replace_nid = $list[0]['nid'];
    $replace_node_title = $list[0]['node_title'];
    foreach ($replace_node_title as $keys => $values) {
      print ("<li>" . l($values, 'node/' . $replace_nid[$keys]) . "</li><ul class='meta_tag_results'>");
      print ("<li> <small>  Meta $meta_field : </small> " . $replace_meta_list[$keys] . "</li></ul>");
    }
  }
}
print ($suffix);
?>