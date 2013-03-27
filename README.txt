INTRODUCTION
------------
Rooms is a family of modules that enable you to manage Booking for hotels, B&Bs
and vacation rentals.

It builds on the Drupal CMS (drupal.org) and its modules.

INSTALLATION
------------
Follow the steps below to install Rooms.

1.  Download the FullCalendar library from http://www.drupalrooms.com/sites/default/files/fullcalendar-1.5.4.zip
and unpack in sites/all/libraries so that you end up with sites/all/libraries/fullcalendar/fullcalendar

We use FullCalendar to display room availability and prices by date in an easily comprehensible manner.

Please do not use the download from the FullCalendar website, as we had to fork the actual FullCalendar library
to add an extra view and provide some simple enhancements that made sense for this application.

*It is not necessary to install a Drupal module called fullcalendar.*

2. Activate all the Rooms modules and their dependencies. Dependencies will be automatically downloaded if
using drush to enable the Rooms modules. Alternatively please make sure you download all dependencies (which can be seen
in the modules page).

At this point the module is installed and what is required is configuration.

CONFIGURATION
-------------

1. Visit admin/rooms/units/unit-types and create a unit type (e.g. standard double room).

2. Visit admin/rooms/units and create a couple of bookable units.

3. Set availability and pricing - clicking on events will bring up a colorbox and allow you to interact with them.

4. Visit admin/commerce/customer-profiles and create a Customer Profile in Drupal Commerce.

5. Create a Booking at admin/rooms/bookings/add

6. Go to /booking and do an availability search.

7. If you wish to display availability information on an embedded calendar in a node (as vacation rentals properties often do) activate the Rooms Availability Reference Module.
7a. This will give you a new field type called "Availability Reference".
7b. Add the field to any entity and you may reference specific Booking Units to have the availability information rendered on the node display (or any other entity).


The results of the availability search are the rooms available over the period - adding one to a cart
will create a Room Booking Unit commerce product for it and place it in the cart. Once checkout is completed
the calendar is updated to reflect the change.

Please join us in the Rooms issue queue and collaborate with us to create a great booking solution for Drupal!

PHP REQUIREMENTS & DATE REQUIREMENTS
--------------------------------------
The use of the DateInterval class means that we require PHP version 5.3 or greater.

In general, this module makes use of latest Date functionality in PHP, so
ensure that it is enabled in your PHP setup. We may scale back on this
to make things more usable for a wider range of configurations.

UPGRADING FROM PREVIOUS BETA VERSIONS
-------------------------------------
If you are upgrading from Beta 4 make sure that your Rooms Product in Commerce has a price set, the price is not
actually used to calculate the cost of the room but we need a price in order for Commerce to properly add the product
to the cart and do tax calculations.

UNINSTALLATION
-------------------------------------
If you wish to uninstall Rooms, you must disable and uninstall the modules it provides in reverse order of dependencies. After disabling each module that is available to be disabled, visit the 'uninstall' tab and uninstall it.
