<?php

/**
 * @file
 * Home of the MySQL instance handler class.
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\Command;
use Drush\psm\InstanceBase;

/**
 * Mysql instance handler class.
 */
class Mysql extends InstanceBase {

  /**
   * {@inheritdoc}
   */
  protected static $executableNames = array(
    'mysql_safe',
  );

  /**
   * {@inheritdoc}
   */
  protected $versionOption = '--help';

  /**
   * {@inheritdoc}
   */
  protected $versionPattern = '/(?<=Ver )(?P<version>\d[^\s]{0,})/';

  /**
   * {@inheritdoc}
   */
  public static function defaultInfo(array $info) {
    $default = array(
      'daemon' => 'background',
      'base_dir' => '/usr',
      'log_file_std' => '/dev/null',
      'log_file_error' => '&1',
    );

    $base_dir = !empty($info['base_dir']) ? $info['base_dir'] : $default['base_dir'];
    $default += array(
      'working_dir' => $base_dir,
      'executable' => "{$base_dir}/bin/mysqld_safe",
      'executable_version' => "{$base_dir}/bin/mysqld",
      'executable_admin' => "{$base_dir}/bin/mysqladmin",
    );

    return $default + parent::defaultInfo($info);
  }

  /**
   * {@inheritdoc}
   */
  protected function getStartCommand() {
    $command = parent::getStartCommand();

    $command->addOptions($this->getInfoEntry('executable_options', FALSE, array()));

    return $command;
  }

  /**
   * {@inheritdoc}
   */
  protected function getStopCommand() {
    $command = new Command();

    $command->workingDir = $this->getInfoEntry('working_dir', FALSE);
    $command->executable = $this->getInfoEntry('executable_admin');

    $info = $this->getInfo();
    if (!empty($info['executable_options']['--defaults-file'])) {
      $command->addOptions(array(
        '--defaults-file' => $info['executable_options']['--defaults-file'],
      ));
    }

    $command->executable .= ' shutdown';

    return $command;
  }

}
