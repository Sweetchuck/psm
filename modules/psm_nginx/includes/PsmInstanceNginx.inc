<?php

/**
 * @file
 * Okay.
 */

class PsmInstanceNginx extends PsmInstanceBase {

  /**
   * @return int
   */
  public function status() {
    return $this->statusByPidFile();
  }

  public function start() {
    $status = $this->status();
    if ($status) {
      return $status;
    }

    $aliases = array(
      'p' => 'prefix',
      'c' => 'config_file',
      'g' => 'directives',
    );

    $options = $this->getInfoEntry('start_options', FALSE, array());
    $options += array_fill_keys($aliases, '');

    $cmd = array($this->getInfoEntry('executable'));
    foreach ($aliases as $option => $alias) {
      if ($options[$option]) {
        $cmd[0] .= " -{$option} %s";
        $cmd[] = $options[$alias];
      }
    }

    _drush_shell_exec($cmd);

    return $this->status();
  }

  public function stop() {
    $cmd = $this->getInfoEntry('executable') . ' -s stop';
    drush_shell_exec($cmd);

    return $this->status();
  }

}
