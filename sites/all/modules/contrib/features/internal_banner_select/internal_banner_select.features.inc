<?php
/**
 * @file
 * internal_banner_select.features.inc
 */

/**
 * Implements hook_views_api().
 */
function internal_banner_select_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function internal_banner_select_node_info() {
  $items = array(
    'internal_page_banners' => array(
      'name' => t('Internal Page Banners'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'page' => array(
      'name' => t('Basic page'),
      'base' => 'node_content',
      'description' => t('Use <em>basic pages</em> for your static content, such as an \'About us\' page.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
