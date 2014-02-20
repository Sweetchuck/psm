<?php

/**
 * @file
 * Service instance definitions for Personal Services Manager.
 *
 * File name: ~/.drush/instances.psm.drushrc.php
 */

/**
 * Add the instance definition to the $instances variable.
 *
 * The first level key is the machine name of the service.
 * To list the available services run:
 * `drush psm-services  --fields=name --field-labels=0`
 *
 * The second level key is a custom machine name of the instance and the value
 * is an array with the properties of the instance.
 * <code>
 * $instances = array(
 *   'mysql' => array(
 *     'my_mysql' => array(
 *       // Key-value pairs of the instance properties.
 *       // The keys are depend on the type of the service, but there are common
 *       // properties.
 *     ),
 *   ),
 * );
 * </code>
 *
 * The common properties:
 *   - label:
 *     Human readable name of the instance.
 *     If empty then the machine name of the instance will be used.
 *     Optional.
 *     Default: <empty>
 *   - description:
 *     Long description of the instance.
 *     Optional.
 *     Default: <generated>
 *   - pid_file:
 *     Filesystem path to the PID file.
 *     Required.
 *   - log_file_std:
 *     Filesystem path to redirect the standard output into. The '&2' is also a
 *     valid value.
 *     Optional.
 *     Default: <empty>
 *   - log_file_error:
 *     Filesystem path to redirect the error output into. The '&1' is also a
 *     valid value.
 *     Optional.
 *     Default: <empty>
 *   - status_delay:
 *     Indicate how many seconds need to wait before check the status of the
 *     instance after the "stop" and "start" commands.
 *     Optional.
 *     Default: Depends on the type of the service.
 *   - executable:
 *     Filesystem path to the binary executable which is handle the service.
 *     Optional.
 *     Default: The default value is detected automatically.
 *     The "/usr/sbin/memcached" will be recognized as a valid executable in
 *     case of the "memcache" service.
 *   - executable_options:
 *     Array of options to pass to the "executable". The values are depend on
 *     the type of the service.
 */
$instances = array();

foreach (array('5.5.5', '5.4.21') as $version) {
  foreach (array('dev', 'test') as $variant) {
    $version_short = str_replace('.', '', $version);
    $prefix = "{$_SERVER['HOME']}/usr/share/php-{$version}";
    $instances['phpfpm']["$version_short-$variant"] = array(
      'label' => "$version $variant",
      'description' => dt('PHP-FPM service v@version with @variant configuration.', array(
          '@version' => $version,
          '@variant' => $variant,
        )),
      'pid_file' => "$prefix/var/run/php-fpm.$variant.pid",
      'executable' => "$prefix/sbin/php-fpm",
      'executable_options' => array(
        'c' => "$prefix/etc/php.$variant.ini",
        'n' => FALSE,
        'd' => '',
        'e' => FALSE,
        'p' => '',
        'g' => FALSE,
        'y' => "$prefix/etc/php-fpm.$variant.conf",
        'D' => FALSE,
        'F' => FALSE,
        'R' => FALSE,
      ),
    );
  }
}

$instances['memcache']['11220'] = array(
  'pid_file' => '/tmp/memcache-11220.pid',
  'executable_options' => array(
    // -p <num>
    // TCP port number to listen on (default: 11211).
    'p' => '11220',
    // -U <num>
    // UDP port number to listen on (default: 11211, 0 is off).
    'U' => FALSE,
    // -s <file>
    // UNIX socket path to listen on (disables network support).
    's' => NULL,
    // -a <mask>
    // Access mask for UNIX socket, in octal (default: 0700)
    'a' => NULL,
    // -l <addr>
    // Interface to listen on (default: INADDR_ANY, all addresses)
    // <addr> may be specified as host:port. If you don't specify
    // a port number, the value you specified with -p or -U is
    // used. You may specify multiple addresses separated by comma
    // or by using -l multiple times.
    'l' => '127.0.0.1',
    // -d
    // Run as a daemon.
    'd' => TRUE,
    // -r
    // Maximize core file limit.
    'r' => FALSE,
    // -u <username>
    // Assume identity of <username> (only when run as root).
    'u' => FALSE,
    // -m <num>
    // Max memory to use for items in megabytes (default: 64 MB).
    'm' => FALSE,
    // -M
    // Return error on memory exhausted (rather than removing items).
    'M' => FALSE,
    // -c <num>
    // Max simultaneous connections (default: 1024).
    'c' => FALSE,
    // -k
    // Lock down all paged memory. Note that there is a
    // limit on how much memory you may lock.  Trying to
    // allocate more than that would fail, so be sure you
    // set the limit correctly for the user you started
    // the daemon with (not for -u <username> user;
    // under sh this is done with 'ulimit -S -l NUM_KB').
    'k' => FALSE,
    // -v
    // Verbose (print errors/warnings while in event loop).
    'v' => FALSE,
    // -vv
    // Very verbose (also print client commands/reponses).
    'vv' => FALSE,
    // -vvv
    // Extremely verbose (also print internal state transitions).
    'vvv' => FALSE,
    // -P <file>
    // Save PID in <file>, only used with -d option.
    'P' => TRUE,
    // -f <factor>
    // Chunk size growth factor (default: 1.25)
    'f' => FALSE,
    // -n <bytes>
    // Minimum space allocated for key+value+flags (default: 48)
    'n' => FALSE,
    // -L
    // Try to use large memory pages (if available). Increasing
    // the memory page size could reduce the number of TLB misses
    // and improve the performance. In order to get large pages
    // from the OS, memcached will allocate the total item-cache
    // in one large chunk.
    'L' => FALSE,
    // -D <char>
    // Use <char> as the delimiter between key prefixes and IDs.
    // This is used for per-prefix stats reporting. The default is
    // ":" (colon). If this option is specified, stats collection
    // is turned on automatically; if not, then it may be turned on
    // by sending the "stats detail on" command to the server.
    'D' => FALSE,
    // -t <num>      number of threads to use (default: 4)
    't' => FALSE,
    // -R
    // Maximum number of requests per event, limits the number of
    // requests process for a given connection to prevent
    // starvation (default: 20)
    'R' => FALSE,
    // -C
    // Disable use of CAS.
    'C' => FALSE,
    // -b
    // Set the backlog queue limit (default: 1024)
    'b' => FALSE,
    // -B
    // Binding protocol - one of ascii, binary, or auto (default).
    'B' => FALSE,
    // -I
    // Override the size of each slab page. Adjusts max item size
    // (default: 1mb, min: 1k, max: 128m)
    'I' => FALSE,
    // -S
    // Turn on Sasl authentication.
    'S' => FALSE,
    // -o
    // Comma separated list of extended or experimental options
    // - (EXPERIMENTAL) maxconns_fast: immediately close new
    //   connections if over maxconns limit
    // - hashpower: An integer multiplier for how large the hash
    //   table should be. Can be grown at runtime if not big enough.
    //   Set this based on "STAT hash_power_level" before a restart.
    'o' => FALSE,
  ),
);

$instances['nginx']['1080'] = array(
  'pid_file' => '/tmp/nginx.pid',
  'executable_options' => array(
    // Set prefix path.
    '-p' => FALSE,
    // Set configuration file.
    '-c' => '/path/to/custom-nginx.conf',
    // Suppress non-error messages during configuration testing.
    '-q' => FALSE,
    // Set global directives out of configuration file.
    '-g' => FALSE,
  ),
);
