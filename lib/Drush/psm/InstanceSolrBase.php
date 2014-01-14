<?php

/**
 * @file
 * Home of the base class for Apache Solr instance manager.
 */

namespace Drush\psm;

/**
 * Apache Solr instance manager.
 *
 * @package Drush\psm
 */
abstract class InstanceSolrBase extends InstanceBasePid {

  protected $solrCoreFileNamePrefix = 'apache-solr-core';

  /**
   * @return string
   */
  public function version() {
    if ($this->versionNumber === NULL) {
      $this->versionNumber = '';
      $solr_core_path = $this->getSolrCoreFilePath();
      $solr_core = pathinfo($solr_core_path, PATHINFO_FILENAME);
      $this->versionNumber = str_replace($this->solrCoreFileNamePrefix . '-', '', $solr_core);
    }

    return $this->versionNumber;
  }

  /**
   * @return string
   */
  protected function getSolrCoreFilePath() {
    $root_dir = $this->getInfoEntry('root_dir');
    $file = reset(drush_scan_directory(
        "$root_dir/dist",
        '/^' . preg_quote($this->solrCoreFileNamePrefix, '/') . '-\d+(\.\d+)*\.jar$/'
      ));

    return $file ? $file->filename : NULL;
  }

}
