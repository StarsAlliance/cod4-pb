THIS IS A BETA BUILD OF COD4. RUN AT YOUR OWN RISK.

Call Of Duty 4(tm)
Linux Multiplayer Server Code
Version 1.7.1
Readme
Last update: 2011-08-01
=============================

!! IMPORTANT !!
 Call of Duty 4(tm) Linux Server is NOT SUPPORTED by Activision(r) Customer
 Support. Please do not call with any questions related to this free beta
 product. There are other channels to aid you listed at the bottom of this
 document.

===============================================

TABLE OF CONTENTS

1. Introduction
2. Upgrading to 1.7.1
3. Installation From Scratch
4. Note for localized versions of the game
5. Support Channels
6. FreeBSD Note
7. Linux-specific version!

===============================================

1. Introduction

 This document explains how to install the Call of Duty 4(tm) Linux server
 version 1.7.1. Installation from scratch and upgrading an existing
 installation are both covered.

 Usage is very similar to other Call of Duty(tm) titles ... many
 of the console commands, command lines, and cvars are identical, so if you
 are comfortable maintaining dedicated servers for those games, you will find
 this process familiar.

 MOD USERS: PLEASE READ...
  It is recommended that any user modifications that have been
  installed to the Call of Duty 4(tm) directory be removed before
  installing this package. These modifications are not supported
  by Activision(r) and may not be compatible with some of the new
  features that are included. When installing or upgrading a server,
  if problems or unexpected behavior arise, your first step in
  troubleshooting should be to do a clean install with the original
  data files.


2. Upgrading to 1.7.1

 Just stop the game server, replace cod4_lnxded-bin, etc on your server with
  the files included in this package, and restart the game server. New files
  may be added, such as main/iw_12.iwd; make sure they land in the correct
  subdirectory. Please verify that your mods are compatible with the new
  version; that is beyond the scope of this document.


3. Installation From Scratch

 - Get the retail Call of Duty 4(tm) disc(s) (there may be multiple discs
   depending on what edition of the game you have obtained, or perhaps a
   single DVD-ROM disc).
 - Copy the contents of disc one's "Setup/Data" directory to wherever you
   want to install the Call of Duty 4(tm) Linux server. There should be a
   "localization.txt" file in the root of this directory, and a "main"
   Subdirectory.  Each additional disc should be opened and the contents of
   each "Data" folder should be copied over to the existing Main folder. When
   you have copied everything, the final installation size is around 6.3
   gigabytes.
 - Alternately, you may install on Windows(r) and copy the installed game to
   your Linux system, but many will opt to skip this step since the data
   files are uncompressed and easily accessible on the discs. Final
   installation size is around 6.3 gigabytes.
 - Please make sure you transfer the data as BINARY and not TEXT or ASCII if
   you use the FTP protocol to upload the files!
 - Unpack this archive in the root of the newly-copied tree, so
   "cod4_lnxded" is in the same directory as "localization.txt". Unlike the
   original Call of Duty(tm), there are not seperate .so files like
   "game.mp.i386.so", so don't be concerned when you don't see them.
   Files such as main/iw_12.iwd need to go into the correct subdirectories;
   make sure they don't lose the path information and all copy into the base
   directory!

 - OPTIONAL: Install PunkBuster:

     ./pbsetup.run -e
     ./pbsetup.run --add-game=cod4 --add-game-path=/where/i/uploaded/cod4/
     ./pbsetup.run -u

   You will need to agree to the PunkBuster license before you may install it.
   This will download the latest version of the Punkbuster software to your
   Linux server, so you need an Internet connection.

 - Now, run the server:
     cd /where/i/copied/callofduty4
     ./cod4_lnxded

 - When you see "--- Common Initialization Complete ---", the game
   server has started, but you need to start a map before the server will
   accept connections. At this point, type:

     map mp_crash

   ("mp_crash" being a given map's name).

 - Now you should see your server in the in-game browser. You will now want to
   customize your server, including writing config files and command lines
   that don't require human interaction, but that is beyond the scope of this
   document.


4. Note for localized versions of the game

 If you have a copy of the game that does not include the English translations,
 you will need to explicitly set the game's language or the server will not
 be able to find its data files. If you have, for example, the French data
 files, you must add this to your server's command line:

   ./cod4_lnxded +set loc_language 1

 This is a list of values for various languages. English installations can
 ignore this.

   English (default): +set loc_language 0
   French: +set loc_language 1
   German: +set loc_language 2
   Italian: +set loc_language 3
   Spanish: +set loc_language 4


5. Support Channels

 There are a LOT of knobs you can tweak to customize and automate your server,
 but it is beyond the scope of this documentation. Please refer to the
 admin manuals for any Quake 3(tm) based Multiplayer game (including Quake 3
 Arena(tm), Return to Castle Wolfenstein(tm), the original Call of Duty(tm)
 and United Offensive(tm), etc) for specifics.

 There is a mailing list for discussion and support of Linux servers for all
 of the Call of Duty(tm) games and expansion packs. Hundreds of experienced
 server admins and even some of the game's developers monitor this list, and
 are eager to help with politely asked questions. Send a blank email to
 cod-subscribe@icculus.org to get on the list, and list archives can be seen
 at:

   http://icculus.org/cgi-bin/ezmlm/ezmlm-cgi?38

 Bug reports should NOT be sent to the list. We have a web-based
 bugtracking system for this. If you don't report bugs there, we don't
 promise to even be aware of them, let alone fix them! You can find the bug
 tracker here:

   https://bugzilla.icculus.org/


 Also, http://callofduty.com/ and http://infinityward.com/ may direct you to
 important information, documentation and current news about Call of Duty(tm)
 titles.


6.  FreeBSD users

 This server is known to work on FreeBSD with the Linux binary compatibility
 layer. If it doesn't, we consider it a bug and appreciate the report since we
 won't necessarily be testing on FreeBSD ourselves.


7. Linux-specific version!

 Version 1.7.1 is Linux-specific hotfix, and is otherwise equivalent to
 version 1.7 on other platforms. The server will report itself as 1.7.
 This patch adds support for query limits.

 About query limits:

  There is a class of DDoS attack that can utilize COD4 servers to flood a
  third party, by spoofing UDP packets so that the game server sends its
  reply for information to an unsuspecting party, over and over, as fast as
  it can. Unlike most packets sent by the server, this reply packet does not
  require a player with a legitimate connection before sending.

  This patch sets up some reasonable defaults to limit the rate at which the
  server will send these reply packets to a given IP address. It does not
  throttle legitimate connections in the process.

  The gist is this: If someone sends a query packet, we note their IP
  address and ignore any further queries for X seconds. If they send a
  port-unreachable packet (person being spoofed isn't playing the game),
  we ignore their IP address for Y seconds. This will let normal people
  play, it prevents people that don't have a legitimate player connection
  from flooding the server with queries, and it'll stop DDoS attacks
  against third parties.

  If millions of computers try to wail on a single server, this patch
  should handle it gracefully (we don't allocate memory when adding IP
  addresses to the ignore list, we use a hashtable so we don't have to
  check millions of IP addresses for every query, and we clean out old
  addresses a little each frame).

  People that are connected to the server don't have their packets
  ignored; this only limits server responses to packets that don't need a
  valid player connection (like the infostring, etc). LAN addresses are
  never limited.

  Admins can tune it (and turn it off completely) with cvars.

  Note that a single IP address using different ports all get lumped into
  the same ignore list entry. This is to make life difficult for attackers
  and keep things efficient on the server. If 16 players are all behind
  the same NAT, they might notice it takes longer for the server to
  respond to each of them individually at the start. They should run a LAN
  server anyhow, but gameplay will still perform normally here.

  Server admins have 4 cvars to control this:

    sv_queryIgnoreDebug: set to 1 to log information about attacks. This will
    write out a lot of logging. Defaults to 0.

    sv_queryIgnoreMegs: Number of megabytes we should use to store the
    ignore list. Set to 0 to turn off the ignore list (basically disables
    this patch). 1 megabyte handles about 65000 IP addresses, each megabyte
    after the first adds about 87000 more. 1 is probably fine unless you're
    under serious attack, but maybe a server wants to spare 12 megs to block
    a million IP addresses simultaneously.  :)  Defaults to 1.

    sv_queryIgnoreTime: Number of milliseconds to ignore an IP address's
    info requests after responding to one of them. Set to 0 to not ignore at
    all. Defaults to 2000 (2 seconds).

    sv_queryBounceIgnoreTime: Number of milliseconds to ignore an IP
    address's info requests after a server packet bounced with an ICMP Port
    Unreachable notice. Set to 0 to not ignore at all. Defaults to 12000 (2
    _minutes_).


// end of README.linux ...

