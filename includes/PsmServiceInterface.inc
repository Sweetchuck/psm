<?php
/**
 * @file
 *
 */

interface PsmServiceInterface {
  function status();
  function start();
  function stop();
  function restart();
}
