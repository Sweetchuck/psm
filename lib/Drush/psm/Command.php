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

  /**
   * Cast to string.
   *
   * @return string
   *   The command.
   */
  public function __toString() {
    $args = $this->build();

    $cmd = array_shift($args);
    foreach ($args as $key => $value) {
      $args[$key] = escapeshellarg($value);
    }

    return vsprintf($cmd, $args);
  }

  /**
   * Check we need to run the command in interactive mode.
   *
   * This is useful for redirects the outputs into a custom file.
   *
   * @return bool
   *   TRUE if we want to suppress the internal output redirect of Drush.
   */
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
    $cmd = $this->build();

    if ($this->workingDir) {
      $cwd = getcwd();
      drush_op('chdir', $this->workingDir);
      $result = _drush_shell_exec($cmd, $this->interactive());
      drush_op('chdir', $cwd);

      return $result;
    }
    else {
      return _drush_shell_exec($cmd, $this->interactive());
    }
  }

  /**
   * Add command line options and their placeholders.
   *
   * @param array $options
   *   Array of command line options.
   */
  public function addOptions(array $options) {
    foreach ($options as $option_name => $option_value) {
      if ($option_value === FALSE
        || $option_value === array()
        || !preg_match('/^[a-zA-Z0-9\._-]+$/', $option_name)
      ) {
        continue;
      }

      switch (gettype($option_value)) {
        case 'boolean':
          $this->executable .= " $option_name";
          break;

        case 'integer':
        case 'double':
        case 'float':
        case 'string':
          $this->executable .= " $option_name=%s";
          $this->arguments[] = $option_value;
          break;

        case 'array':
          $this->executable .= " $option_name=%s";
          $this->arguments[] = implode(',', $option_value);
          break;

      }
    }
  }

  /**
   * Assemble the command string fragments into one array.
   *
   * @return array
   *   The first item is the command string with the placeholders of the
   *   arguments in it. And the rest items are the placeholder values.
   */
  protected function build() {
    $cmd = array(
      'executable' => $this->executable,
    );

    $cmd = array_merge($cmd, $this->arguments);

    if ($this->daemon === 'nohup') {
      $cmd['executable'] = 'nohup ' . $cmd['executable'] . ' &';
    }

    if (!empty($this->redirectStd)) {
      if ($this->redirectStd == '&2') {
        $cmd['executable'] .= ' >&2';
      }
      else {
        $cmd['executable'] .= ' > %s';
        $cmd[] = $this->redirectStd;
      }
    }

    if (!empty($this->redirectError)) {
      if ($this->redirectError == '&1') {
        $cmd['executable'] .= ' 2>&1';
      }
      else {
        $cmd['executable'] .= ' 2> %s';
        $cmd[] = $this->redirectError;
      }
    }

    if ($this->daemon === 'background') {
      $cmd['executable'] .= ' &';
    }

    if (!empty($this->pidFile)) {
      $cmd['executable'] .= ' && echo $! > %s';
      $cmd[] = $this->pidFile;
    }

    // Numeric indexed array.
    return array_values($cmd);
  }
}
