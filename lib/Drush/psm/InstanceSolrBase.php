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
abstract class InstanceSolrBase extends InstanceBase {

  /**
   * @var string
   */
  protected $solrCoreFileNamePrefix = 'apache-solr-core';

  /**
   * {@inheritdoc}
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
   * {@inheritdoc}
   */
  protected function getStartCommand() {
    $command = parent::getStartCommand();

    $options = $this->getInfoEntry('executable_options');
    foreach ($options as $option_name => $option_value) {
      if ($option_value === FALSE || $option_value === array()) {
        continue;
      }

      $command->executable .= " $option_name=%s";
      $command->arguments[] = $option_value;
    }

    $command->executable = ' -jar %s';
    $command->arguments[] = $this->getInfoEntry('jar');

    $command->pidFile = $this->getPidFile();

    return $command;
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
