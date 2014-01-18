<?php

/**
 * @file
 * Home of the InstanceBase class.
 */

namespace Drush\psm;

/**
 * Class InstanceBase.
 *
 * @package Drush\psm
 */
abstract class InstanceBase implements InstanceInterface {

  /**
   * Configuration reload.
   *
   * Start the new worker processes with a new configuration.
   * Gracefully shutdown the old worker processes.
   */
  const SIGNAL_RELOAD = 1;

  /**
   * Will stop the Nginx server.
   */
  const SIGNAL_STOP = 2;

  /**
   * Graceful shutdown.
   */
  const SIGNAL_QUIT = 3;

  /**
   * Array to store the instances.
   *
   * @var array
   */
  protected static $instances = array();

  /**
   * Seconds to wait before check the status of the instance.
   *
   * @var int
   */
  protected static $defaultStatusDelay = 0;

  /**
   * Instance definition.
   *
   * @var array
   */
  protected $info = array();

  /**
   * Command line options after the executable to retrieve an output which is
   * contains the version number.
   *
   * @var string
   */
  protected $versionOption = '-v';

  /**
   * Regular expression to identify the version number.
   *
   * @var string
   */
  protected $versionPattern = '/(?P<version>\d[^\s]{0,})/';

  /**
   * Cached version number.
   *
   * @var string
   */
  protected $versionNumber = NULL;

  /**
   * {@inheritdoc}
   */
  public static function factory(array $info) {
    if (!array_key_exists($info['name'], static::$instances)) {
      static::$instances[$info['name']] = new static($info);
    }

    return static::$instances[$info['name']];
  }

  /**
   * Create an instance handler.
   *
   * @param array $info
   *   Instance definition.
   */
  protected function __construct(array $info) {
    $this->info = $info;
  }

  /**
   * {@inheritdoc}
   */
  public function id() {
    return $this->service() . '.' . $this->name();
  }

  /**
   * {@inheritdoc}
   */
  public function service() {
    return $this->getInfoEntry('service');
  }

  /**
   * Get the instance definition.
   *
   * @return array
   *   Instance definition.
   */
  public function getInfo() {
    return $this->info;
  }

  /**
   * {@inheritdoc}
   */
  public function name() {
    return $this->getInfoEntry('name');
  }

  /**
   * {@inheritdoc}
   */
  public function label() {
    return $this->getInfoEntry('label');
  }

  /**
   * {@inheritdoc}
   */
  public function description() {
    return $this->getInfoEntry('description');
  }

  /**
   * {@inheritdoc}
   */
  public function version() {
    if ($this->versionNumber === NULL) {
      $this->versionNumber = '';
      $cmd = escapeshellcmd($this->getInfoEntry('executable', FALSE));
      if ($cmd && $this->versionOption !== NULL) {
        if (!is_executable($cmd)) {
          throw new \Exception('Not executable: ' . $cmd, 1);
        }

        $cmd .= ' ' . $this->versionOption;
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
  public function status($delay = FALSE) {
    if ($delay === TRUE) {
      $delay = $this->getStatusDelay();
    }

    if ($delay) {
      sleep((int) $delay);
    }

    return $this->isPidRunning();
  }

  /**
   * {@inheritdoc}
   */
  public function start() {
    $status = $this->status();
    if ($status) {
      return $status;
    }

    return $this->getStartCommand()->run() && $this->status(TRUE);
  }

  /**
   * {@inheritdoc}
   */
  public function stop() {
    if (!$this->status()) {
      return TRUE;
    }

    return $this->getStopCommand()->run() && !$this->status(TRUE);
  }

  /**
   * {@inheritdoc}
   */
  public function reload() {
    if (!$this->status()) {
      return FALSE;
    }

    return $this->getReloadCommand()->run() && $this->status(TRUE);
  }

  /**
   * {@inheritdoc}
   */
  public function restart() {
    $this->stop();

    return $this->start();
  }

  /**
   * Check the process is running or not.
   *
   * @return int|bool
   *   The process ID or FALSE.
   */
  public function isPidRunning() {
    $pid = $this->getPid();
    if (!$pid) {
      return FALSE;
    }

    $cmd = sprintf('kill -s 0 %d > /dev/null 2>&1', $pid);
    $return_var = NULL;
    $output = NULL;
    exec($cmd, $output, $return_var);

    if ($return_var === 0) {
      // @todo Check the process is belong to this instance. getCmdLine().
      return $pid;
    }

    $this->cleanPidFile();

    return FALSE;
  }

  /**
   * Get the process ID.
   *
   * @return int
   *   The process ID.
   */
  public function getPid() {
    $pid_file = $this->getPidFile();
    if (!$pid_file || !is_readable($pid_file)) {
      return 0;
    }

    $pid = trim(file_get_contents($pid_file));

    return preg_match('/^\d+$/', $pid) ? (int) $pid : 0;
  }

  /**
   * Get the file path of the PID file.
   *
   * @return string
   *   File system path to PID file.
   */
  public function getPidFile() {
    return $this->getInfoEntry('pid_file');
  }

  /**
   * Get the executed command.
   *
   * @return string|bool
   *   Command.
   */
  public function getCmdLine() {
    $pid = $this->getPid();
    if (!$pid) {
      return FALSE;
    }

    $file_name = "/proc/$pid/cmdline";
    if (!is_readable($file_name)) {
      return FALSE;
    }

    return (drush_shell_exec("cat $file_name")) ?
      implode("\n", (array) drush_shell_exec_output()) : FALSE;
  }

  /**
   * Get an entry from the instance definition.
   *
   * @param string $key
   *   Array key.
   * @param bool $required
   *   Indicate the $key must be exists or not.
   * @param mixed $default
   *   If the existence is optional the this will be the default value.
   *
   * @throws \Exception
   * @return mixed
   *   Value of the $key.
   */
  protected function getInfoEntry($key, $required = TRUE, $default = NULL) {
    $info = $this->getInfo();

    if (!array_key_exists($key, $info)) {
      if ($required) {
        throw new \Exception("Invalid argument: $key", 1);
      }
      else {
        return $default;
      }
    }

    return $info[$key];
  }

  /**
   * Get the amount of delay seconds.
   *
   * @return int
   *   Amount of seconds.
   */
  protected function getStatusDelay() {
    return $this->getInfoEntry(
      'status_delay', FALSE, static::$defaultStatusDelay
    );
  }

  /**
   * Basic process handler.
   *
   * @param int $signal
   *   Reload, stop.
   *
   * @return bool|null
   *   Status of the command execution.
   */
  protected function sendSignal($signal) {
    $executable = $this->getInfoEntry('executable');
    $pid_file = $this->getPidFile();
    $result = NULL;
    switch ($signal) {
      case static::SIGNAL_RELOAD:
        // @todo Configurable path to killproc.
        $result = drush_shell_exec('/sbin/killproc -HUP -p %s %s', $pid_file, $executable);
        break;

      case static::SIGNAL_QUIT:
      case static::SIGNAL_STOP:
        $result = drush_shell_exec('/sbin/killproc -TERM -p %s %s', $pid_file, $executable);
        break;

    }

    return $result;
  }

  /**
   * Check the status of the instance by the PID file.
   *
   * @deprecated Use directly the isPidRunning() method.
   *
   * @return int
   *   Process ID.
   */
  protected function statusByPidFile() {
    return $this->isPidRunning();
  }

  /**
   * Get the command to start the instance.
   *
   * @return Command
   *   Command object to start the instance.
   */
  protected function getStartCommand() {
    $command = new Command();

    $command->workingDir = $this->getInfoEntry('working_dir', FALSE, '');
    $command->executable = $this->getInfoEntry('executable');
    $command->redirectStd = $this->getInfoEntry('log_file_std', FALSE, '');
    $command->redirectError = $this->getInfoEntry('log_file_error', FALSE, '');

    return $command;
  }

  /**
   * Get the command to stop the instance.
   *
   * @return Command
   *   Command object to stop the instance.
   */
  protected function getStopCommand() {
    $command = new Command();

    $command->executable = '/sbin/killproc -TERM -p %s %s';
    $command->arguments[] = $this->getPidFile();
    $command->arguments[] = $this->getInfoEntry('executable');

    return $command;
  }

  /**
   * Get the command to reload the instance.
   *
   * @return Command
   *   Command object to reload the instance.
   */
  protected function getReloadCommand() {
    $command = new Command();

    $command->executable = '/sbin/killproc -HUP -p %s %s';
    $command->arguments[] = $this->getPidFile();
    $command->arguments[] = $this->getInfoEntry('executable');

    return $command;
  }

  protected function cleanPidFile() {
    $pid_file = $this->getPidFile();
    if ($pid_file) {
      unlink($pid_file);
    }
  }

}
