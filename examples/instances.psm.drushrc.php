<?php

/**
 * @file
 * Service instance definitions for Personal Services Manager.
 *
 * This file must be in this directory: ~/.drush
 */

foreach (array('5.5.5', '5.4.21', '5.3.27', '5.3.17', '5.3.10') as $version) {
  foreach (array('dev', 'test') as $variant) {
    $version_short = str_replace('.', '', $version);
    $prefix = "{$_SERVER['HOME']}/usr/share/php-{$version}";
    $instances['phpfpm']["$version_short-$variant"] = array(
      'label' => "$version $variant",
      'description' => dt('PHP-FPM service v@version with @variant configuration.', array(
          '@version' => $version,
          '@variant' => $variant,
        )),
      'status_delay' => 3,
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
  'label' => '11220',
  'description' => dt('Long description'),
  'pid_file' => "/mnt/tmpfs{$_SERVER['HOME']}/var/run/memcache-11220.pid",
  'executable' => '/usr/sbin/memcached',
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
  'label' => '1080',
  'description' => dt('General Nginx service on port 1080.'),
  'executable' => "{$_SERVER['HOME']}/usr/sbin/nginx",
  'pid_file' => "{$_SERVER['HOME']}/var/run/nginx.pid",
  'executable_options' => array(
    'p' => '',
    'c' => '',
    'q' => '',
  ),
);
