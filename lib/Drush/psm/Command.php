<?php

/**
 * @file
 * Home of the Command class.
 */

namespace Drush\psm;

/**
 * Assemble a command and run it.
 */
class Command {

  /**
   * @var string
   */
  public $workingDir = '';

  /**
   * @var string
   */
  public $daemon = '';

  /**
   * @var string
   */
  public $executable = '';

  /**
   * @var array
   */
  public $arguments = array();

  /**
   * @var string
   */
  public $redirectStd = '';

  /**
   * @var string
   */
  public $redirectError = '';

  /**
   * @var string
   */
  public $pidFile = '';

  /**
   * @var bool
   */
  public $interactive = FALSE;

  public function interactive() {
    return $this->interactive || $this->redirectStd || $this->redirectError;
  }

  /**
   * Run the command.
   *
   * @return bool
   *   TRUE on success, FALSE on failure.
   */
  public function run() {
    $cmd = array();

    $cmd['executable'] = $this->executable;

    $cmd = array_merge($cmd, $this->arguments);

    switch ($this->daemon) {
      case 'background':
        $cmd['executable'] .= ' &';
        break;

      case 'nohup':
        $cmd['executable'] = 'nohup ' . $cmd['executable'] . ' &';
        break;

    }

    if (!empty($this->redirectStd)) {
      if ($this->redirectStd == '&2') {
        $cmd['executable'] .= ' >&2';
      }
      else {
        $cmd['executable'] .= ' >%s';
        $cmd[] = $this->redirectStd;
      }
    }

    if (!empty($this->redirectError)) {
      if ($this->redirectError == '&1') {
        $cmd['executable'] .= ' 2>&1';
      }
      else {
        $cmd['executable'] .= ' 2>%s';
        $cmd[] = $this->redirectError;
      }
    }

    if (!empty($this->pidFile)) {
      $cmd['executable'] .= ' && echo $! > %s';
      $cmd[] = $this->pidFile;
    }

    // Numeric indexed array.
    $cmd = array_values($cmd);

    if ($this->workingDir) {
      $cwd = getcwd();
      drush_op('chdir', $this->workingDir);
      $result = call_user_func('_drush_shell_exec', $cmd, $this->interactive());
      drush_op('chdir', $cwd);

      return $result;
    }
    else {
      return _drush_shell_exec($cmd, $this->interactive());
    }
  }
}
