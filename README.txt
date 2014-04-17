INTRODUCTION
------------
Rooms is a family of modules that enable you to manage Booking for hotels,
B&Bs and vacation rentals.

It builds on the Drupal CMS (drupal.org) and its modules.


INSTALLATION
------------
Follow the steps below to install Rooms.

1. Download the FullCalendar library from [1] and unpack in `sites/all/libraries`
so that you end up with
`sites/all/libraries/rooms_fullcalendar/fullcalendar/fullcalendar.js`.

Rooms uses FullCalendar to display room availability and prices by date in an
easily comprehensible manner.

Please do not use the download from the FullCalendar website, as Rooms
requires a custom fork of actual FullCalendar library which provides a extra
view and some simple enhancements that make sense for this application.

*It is not necessary to install a Drupal module called fullcalendar.*

2. Activate all the Rooms modules and their dependencies. Dependencies will
be automatically downloaded if using [drush][2] to enable the Rooms modules.
Alternatively please make sure you download all dependencies (which can be
seen in the modules page).

At this point the module is installed and you should proceed with configuration.


CONFIGURATION
-------------

1. Visit `admin/rooms/units/unit-types` and create a unit type (e.g. standard
double room).

2. Visit `admin/rooms/units` and create a couple of bookable units.

3. Set availability and pricing - clicking on events or date ranges in the
calendars will bring up a pop-up dialog and allow you to interact with them.

4. To manually create a Booking go to `admin/rooms/bookings/add`. This will also
 create a Commerce order and line item for that booking.

5. Potential guests can go to `/booking` to do an availability search.

6. If you wish to display availability information on an embedded calendar
in a node (as vacation rentals properties often do) activate the Rooms
Availability Reference Module.

6a. This will give you a new field type called "Availability Reference".

6b. Add the field to any entity and you may reference specific Booking Units
to have the availability information rendered on the node display (or any
other entity).

The results of the availability search are the rooms available over the
period - adding one to a cart will create a Room Booking Unit commerce
product for it and place it in the cart. Once checkout is completed the calendar
is updated to reflect the change.

Please join us in the [Rooms issue queue][3] and collaborate with us to create a
great booking solution for Drupal!


PHP REQUIREMENTS & DATE REQUIREMENTS
--------------------------------------
The use of the DateInterval class means that we require PHP version 5.3
or greater.

In general, this module makes use of latest Date functionality in PHP, so
ensure that it is enabled in your PHP setup. We may scale back on this
to make things more usable for a wider range of configurations.


UPGRADING FROM PREVIOUS VERSIONS
-------------------------------------
Please make sure you test extensively on a test site before upgrading!

UNINSTALLATION
-------------------------------------
If you wish to uninstall Rooms, you must disable and uninstall the modules
it provides in reverse order of dependencies. After disabling each module
that is available to be disabled, visit the 'uninstall' tab and uninstall it.

   [1]: http://www.drupalrooms.com/sites/default/files/fullcalendar-1.5.4.zip "Rooms Fullcalendar fork"
   [2]: http://drupal.org/project/drush "Drush, the Drupal shell"
   [3]: http://drupal.org/project/issues/rooms "Rooms issue queue on Drupal.org"
