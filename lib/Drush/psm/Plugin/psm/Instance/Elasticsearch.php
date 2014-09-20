<?php

/**
 * @file
 * Home of the Elasticsearch instance manager class.
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\Command;
use Drush\psm\InstanceBase;

/**
 * Elasticsearch instance manager.
 */
class Elasticsearch extends InstanceBase {

  /**
   * {@inheritdoc}
   */
  protected static $executableNames = array(
    'elasticsearch',
  );

  /**
   * {@inheritdoc}
   */
  protected $versionPattern = '/(?P<version>\d[^,]{0,})/';

  /**
   * {@inheritdoc}
   */
  protected function getStartCommand() {
    $command = parent::getStartCommand();

    $pid_file = $this->getPidFile();
    if ($pid_file) {
      $options = array('-p' => $pid_file);
      $command->addOptions($options, array('key_value_separator' => ' '));
    }

    $options = $this->getInfoEntry('executable_options', FALSE, array());
    $command->addOptions($options);

    return $command;
  }

  /**
   * {@inheritdoc}
   */
  protected function getStopCommand() {
    $command = new Command();

    $command->executable = '/sbin/killproc -TERM -p %s %s';
    $command->arguments[] = $this->getPidFile();
    $command->arguments[] = $this->getStopCommandExecutable();

    return $command;
  }

  /**
   * Get the Java home path.
   *
   * @return string
   *   Java home path.
   */
  protected function getStopCommandExecutable() {
    $suffix = '/bin/java';
    $java_home = (!empty($_SERVER['JAVA_HOME']) ?
      $_SERVER['JAVA_HOME'] : trim(`which java`)
    );

    return $java_home ? $java_home . $suffix : FALSE;
  }

}
