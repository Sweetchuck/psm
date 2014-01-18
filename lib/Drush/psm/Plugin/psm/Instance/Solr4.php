<?php

/**
 * @file
 * Home of the Apache Solr instance manager class.
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\Command;
use Drush\psm\InstanceSolrBase;

/**
 * Apache Solr instance manager.
 *
 * @package Drush\psm\Plugin\psm\Instance
 */
class Solr4 extends InstanceSolrBase {

  /**
   * {@inheritdoc}
   */
  protected $solrCoreFileNamePrefix = 'solr-core';

  /**
   * {@inheritdoc}
   */
  public function version() {
    if ($this->versionNumber === NULL) {
      // Set the version number based on the file name.
      $solr_core_path = $this->getSolrCoreFilePath();
      $solr_core = pathinfo($solr_core_path, PATHINFO_FILENAME);
      $this->versionNumber = str_replace($this->solrCoreFileNamePrefix . '-', '', $solr_core);

      // Try to run a command and parse the output in order to get the version
      // number.
      $cmd = array(
        'working_dir' => $this->getInfoEntry('working_dir'),
        'cmd' => $this->getInfoEntry('executable') . ' -jar %s path=%s --version',
        'jar' => $this->getInfoEntry('jar'),
        'solr-core' => $solr_core_path,
      );
      if (call_user_func_array('drush_shell_cd_and_exec', $cmd)) {
        $pattern = '@\s*\d+:\s+(?P<version>[^\s]+).+? | ' . preg_quote($cmd['solr-core'], '@') . '$@';
        $lines = (array) drush_shell_exec_output();
        foreach ($lines as $line) {
          $matches = array();
          if (preg_match($pattern, $line, $matches)) {
            $this->versionNumber = $matches['version'];
            break;
          }
        }
      }
    }

    return $this->versionNumber;
  }

  /**
   * {@inheritdoc}
   */
  protected function getStartCommand() {
    $command = new Command();

    $command->daemon = $this->getInfoEntry('daemon', FALSE, '');
    $command->workingDir = $this->getInfoEntry('working_dir', FALSE, '');
    $command->executable = $this->getInfoEntry('executable');
    $command->redirectStd = $this->getInfoEntry('log_file_std', FALSE, '');
    $command->redirectError = $this->getInfoEntry('log_file_error', FALSE, '');

    $this->addOptions($command, $this->getInfoEntry('jvm_options', FALSE, array()));

    $command->executable .= ' -jar %s';
    $command->arguments[] = $this->getInfoEntry('jar');

    $this->addOptions($command, $this->getInfoEntry('executable_options', FALSE, array()));

    $pid_file = $this->getInfoEntry('pid_file');
    if ($pid_file) {
      $command->pidFile = $pid_file;
    }

    return $command;
  }

  /**
   * {@inheritdoc}
   */
  protected function getStopCommand() {
    $command = new Command();

    $command->workingDir = $this->getInfoEntry('working_dir', FALSE, '');
    $command->executable = $this->getInfoEntry('executable');

    $this->addOptions($command, $this->getInfoEntry('jvm_options', FALSE, array()));

    $command->executable .= ' -jar %s --stop';
    $command->arguments[] = $this->getInfoEntry('jar');

    return $command;
  }

  protected function addOptions($command, $options) {
    foreach ($options as $option_name => $option_value) {
      if ($option_value === FALSE || $option_value === array()) {
        continue;
      }

      switch ($option_name) {
        // Flag.
        case '--exec':
        case '--daemon':
        case '-DDEBUG':
          $command->executable .= " $option_name";
          break;

        // Key-value.
        case '--config':
        case '--init':
        case '--pre':
        case '-Dpath':
        case '-Dlib':
        case '-DSTOP.PORT':
        case '-DSTOP.KEY':
        case '-DSTOP.WAIT':
        case '-DOPTION':
          if (is_array($option_value)) {
            $option_value = implode(',', $option_value);
          }

          $command->executable .= " $option_name=%s";
          $command->arguments[] = $option_value;
          break;

      }
    }
  }

}
