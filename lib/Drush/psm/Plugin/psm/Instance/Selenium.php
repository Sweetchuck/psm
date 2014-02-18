<?php

/**
 * @file
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\Command;
use Drush\psm\InstanceBase;

/**
 * Selenium instance handler class.
 */
class Selenium extends InstanceBase {

  /**
   * {@inheritdoc}
   */
  protected static $executableNames = array(
    'selenium-server-standalone.jar',
  );

  /**
   * {@inheritdoc}
   */
  public static function defaultInfo(array $info) {
    return array(
      'daemon' => 'nohup',
    ) + parent::defaultInfo($info);
  }

  /**
   * {@inheritdoc}
   */
  public function version() {
    if ($this->versionNumber === NULL) {
      $this->versionNumber = '';
      $executable = realpath($this->getInfoEntry('jar'));
      $matches = array();
      if (preg_match('/(?P<version>\d.*?)\.jar$/', pathinfo($executable, PATHINFO_BASENAME), $matches)) {
        $this->versionNumber = $matches['version'];
      }
    }

    return $this->versionNumber;
  }

  /**
   * {@inheritdoc}
   */
  protected function getStartCommand() {
    $command = parent::getStartCommand();

    $command->addOptions($this->getInfoEntry('jvm_options', FALSE, array()));

    $command->executable .= ' -jar %s';
    $command->arguments[] = $this->getInfoEntry('jar');

    $command->addOptions($this->getInfoEntry('executable_options', FALSE, array()));

    $command->pidFile = $this->getInfoEntry('pid_file');

    return $command;
  }
}
