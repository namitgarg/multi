<?php

/**
 * 
 * @param type $command
 * @return type
 */
function metatag_import_export_csv_drush_help($command) {
    switch ($command) {
        case 'drush:metatag-import':
            return dt('This commans helps you to import metatag to node pages from a csv file
          The argument fot the command is filename
          follow the exampees'
            );
    }
}

function metatag_import_export_csv_type_drush()
{
define('DRUPAL_ROOT', getcwd());
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

 $typedata = node_type_get_types();
  foreach ($typedata as $values1) {
    $type_machine_name[$values1->type] = $values1->type;
  }
    $default_type = array('all' => 'all');
  $content_type = array_merge($default_type, $type_machine_name);
  return $content_type;
}
/**
 * Implements hook_drush_command().
 */
function metatag_import_export_csv_drush_command() { 
    $items['metatag-import'] = array(
      'description' => dt("Import metatags form a csv"),
      'arguments' => array(
        'filepath' => dt("The filepath to your file"),
      ),
      'examples' => array(
        'windows_example' => 'drush metatag-import D:\\drupal\\multi.csv',
      ),
    );
    $items['metatag-export'] = array(
      'description' => dt("Export  metatags to a csv file"),
      'arguments' => array(
        'type' => dt("The type of option"),
      ),
      'examples' => array(
        'windows_example' => 'drush metatag-import D:\\drupal\\multi.csv',
      ),
      'arguments' => array(
       'filepath' => 'The type of statement (error or success).',
      ),
      'options' =>metatag_import_export_csv_type_drush(),
    );
    return $items;
}

function drush_metatag_import_export_csv_metatag_export($type,$filepath)
{
    
    //print("namit garg $filepath");
    if(empty($type))
    {
        $type = drush_choice(metatag_import_export_csv_type_drush(), 'Select the content type you want to export metatags');
    }
    if(!(in_array($type,metatag_import_export_csv_type_drush())))
    {
drush_set_error(dt('@type is not a machine name of any Content type',array('@type' => $type)));
    }
    else{
      include_once 'metatag_import_export_csv_download_admin.inc';
        metatag_import_export_csv_download_nid($type,$filepath);

    }
//$value = drush_prompt(dt('Give the path for the file to be created'));
//drush_print(dt('Path is  @value!', array('@value' =>  $value)));
}


function drush_metatag_import_export_csv_metatag_import
($filepath) {
    if (file_exists($filepath)) {

        $file_info = pathinfo($filepath);
        $file_extension = $file_info['extension'];
        if ($file_extension == 'csv') {
            drush_confirm('Are you sure you want to import metatags this will replace your old metatags ?', $indent = 0);
            module_load_include('inc', 'metatag_import_export_csv', 'metatag_import_export_csv_upload_admin');
            metatag_import_export_csv_read_file($filepath);
        }
        else {
            drush_set_error(dt('File type is not CSV .'));
        }
    }
    else {
        drush_set_error(dt('File with the given path does not exists.'));
    }
}
