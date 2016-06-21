<?php

/**
 * Implements hook_drush_command().
 */
function metatag_import_export_csv_drush_command() {
    $items['metatag-import'] = array(
      'description' => 'Import metatags form a csv',
      'aliases' => array('mi'),
      'arguments'   => array(
      'filepath'     => "filepath",
    ),
    );
    return $items;
}

function drush_metatag_import_export_csv_metatag_import($fileurl) {
    module_load_include('inc', 'metatag_import_export_csv', 'metatag_import_export_csv_upload_admin');
//    print_r($filename);
//    $handle = fopen($filename, "r");
//    $data=fgetcsv($handle);
//    print_r($data);
metatag_import_export_csv_read_file1($fileurl);
}
function metatag_import_export_csv_read_file1($fileurl)
{
        module_load_include('inc', 'metatag_import_export_csv', 'metatag_import_export_csv_upload_admin');
metatag_import_export_csv_read_file($fileurl);

}