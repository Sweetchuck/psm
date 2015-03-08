<?php

/**
 * @file
 * Home of the Php-Fpm instance manager class.
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\Command;

/**
 * PhpFpm instance manager.
 */
class Phpfpm52 extends Phpfpm {

  /**
   * {@inheritdoc}
   */
  protected function getStartCommand() {
    $command = new Command();

    $command->executable = $this->getInfoEntry('executable');
    $command->executable .= ' %s';
    $command->arguments[] = 'start';

    return $command;
  }
  /**
   * {@inheritdoc}
   */
  protected function getStopCommand() {
    $command = new Command();

    $command->executable = $this->getInfoEntry('executable');
    $command->executable .= ' %s';
    $command->arguments[] = 'stop';

    return $command;
  }

  /**
   * {@inheritdoc}
   */
  public function reload() {
    $pid = $this->status();
    if ($pid) {
      $command = new Command();

      $command->executable = $this->getInfoEntry('executable');
      $command->executable .= ' %s';
      $command->arguments[] = 'reload';

      return $command->run() && $this->status(TRUE);
    }

    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function restart() {
    $pid = $this->status();
    if ($pid) {
      $command = new Command();

      $command->executable = $this->getInfoEntry('executable');
      $command->executable .= ' %s';
      $command->arguments[] = 'restart';

      return $command->run() && $this->status(TRUE);
    }

    return FALSE;
  }

}
