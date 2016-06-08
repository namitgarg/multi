<?php

/**
 * Implements hook_drush_command().
 */
function cache_check_drush_command() {
    $items['develop-cache'] = array(
      'description' => 'Check if the cache setting is corrct or not',
      'aliases' => array('dc'),
    );
    return $items;
}

function drush_cache_check_develop_cache() {
    $block_cache = variable_get('block_cache');
    $page_cache = variable_get('cache');
    $page_cache_maximum_age = variable_get('page_cache_maximum_age');
    $preprocess_css = variable_get('preprocess_css');
    $preprocess_js = variable_get('preprocess_js');
    $page_compression = variable_get('page_compression');
    if (($block_cache != '1') || ($page_cache != '1') || ($page_cache_maximum_age != '3600') || ($preprocess_css != '1') || ($preprocess_js != '1')||($page_compression !='0')) {
        print("Error in settings");
        if(!isset($page_compression))
    cache_check_vset('block_cache','1');
    cache_check_vset('page_cache','1');
    cache_check_vset('page_cache_maximum_age','3600');
    cache_check_vset('preprocess_css','1');
    cache_check_vset('preprocess_js','1');
    cache_check_vset('page_compression','0');
        
    }
    else {
        print("correct setting");
    }
}


function cache_check_vset($name,$value)
{
     db_merge('variable')->key(array('name' => $name))->fields(array('value' => serialize($value)))->execute();
print("setting corrected");
}