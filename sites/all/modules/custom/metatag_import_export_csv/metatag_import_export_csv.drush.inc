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

function metatag_import_export_csv_type()
{
    $nam=array("namit"=>"NAMIT","garg"=>"GARG");
               // module_load_include('inc', 'metatag_import_export_csv', 'metatag_import_export_csv_download_admin');
                include_once 'metatag_import_export_csv_download_admin.inc';
               $result= metatag_import_export_csv_type_list();
               print_r($result);

    return $nam;
}
/**
 * Implements hook_drush_command().
 */
function metatag_import_export_csv_drush_command() { 
    $items['metatag-upload'] = array(
      'description' => dt("Import metatags form a csv"),
      'arguments' => array(
        'filepath' => dt("The filepath to your file"),
      ),
      'examples' => array(
        'windows_example' => 'drush metatag-import D:\\drupal\\multi.csv',
      ),
    );
    $items['metatag-download'] = array(
      'description' => dt("Export  metatags to a csv file"),
      'arguments' => array(
        'type' => dt("The type of option"),
      ),
      'examples' => array(
        'windows_example' => 'drush metatag-import D:\\drupal\\multi.csv',
      ),
      'options' =>metatag_import_export_csv_type(),
    );
    return $items;
}

function drush_metatag_import_export_csv_metatag_download($type)
{
    print("import drupal metatags $type");
    
}
function drush_metatag_import_export_csv_metatag_upload($filepath) {
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
