<?php

/**
 * @file
 * Home of the Tomcat instance manager class.
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\Command;
use Drush\psm\InstanceBase;

/**
 * Tomcat instance manager.
 */
class Tomcat extends InstanceBase {

  /**
   * {@inheritdoc}
   */
  protected static $defaultStatusDelay = 5;

  /**
   * {@inheritdoc}
   */
  protected $versionOption = 'version';

  /**
   * {@inheritdoc}
   */
  protected $versionPattern = '/^Server number: *(?P<version>.*?)$/m';

  /**
   * {@inheritdoc}
   */
  public function version() {
    if ($this->versionNumber === NULL) {
      $this->versionNumber = '';
      $cmd = $this->getInfoEntry('executable_version', FALSE, $this->getInfoEntry('executable', FALSE));
      if ($cmd && $this->versionOption !== NULL) {
        if (!is_executable($cmd)) {
          throw new \Exception('Not executable: ' . $cmd, 1);
        }

        $environment = $this->buildEnvironment(array('JSVC'));
        if ($environment) {
          $environment .= ' ';
        }

        $cmd = $environment . escapeshellcmd($cmd) . ' ' . $this->versionOption;
        if (drush_shell_exec($cmd)) {
          $output = implode("\n", (array) drush_shell_exec_output());
          $matches = array('version' => '');
          preg_match($this->versionPattern, $output, $matches);

          $this->versionNumber = $matches['version'];
        }
      }
    }

    return $this->versionNumber;
  }

  /**
   * {@inheritdoc}
   */
  protected function getStartCommand() {
    $command = parent::getStartCommand();

    $env = $this->buildEnvironment();
    if ($env) {
      $command->executable = $env . ' ' . $command->executable;
    }

    $options = array('start' => TRUE);
    $command->addOptions($options);

    return $command;
  }

  /**
   * {@inheritdoc}
   */
  protected function getStopCommand() {
    $command = parent::getStartCommand();

    $env = $this->buildEnvironment();
    if ($env) {
      $command->executable = $env . ' ' . $command->executable;
    }

    $options = array('stop' => TRUE);
    $command->addOptions($options);

    return $command;
  }

}
