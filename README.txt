INTRODUCTION
------------
Rooms is a family of modules that enable you to manage Booking for hotels, B&Bs
and vacation rentals.

It builds on the Drupal CMS (drupal.org) and its modules.

To try out Rooms in this early stage do the following:

1.  Download the FullCalendar library from http://www.drupalrooms.com/sites/default/files/fullcalendar-1.5.3.zip and place in sites/all/libraries so that you end up with sites/all/libraries/fullcalendar/fullcalendar

We use FullCalendar to display room availability and prices by date in an easily comprehensible manner.

Please do not use the download from the FullCalendar website, as we have
added an extra view to the code. Once it has been tested more
extensively, this modification will be submitted as a merge request back
into the original FullCalendar Library. It's not necessary to
install the contributed Drupal fullcalendar module.

2. Install the colorbox module - http://drupal.org/project/colorbox

2a. Follow the colorbox module instructions to install the module and colorbox library.
2b. Go to admin/config/media/colorbox and enable Colorbox load (under extra settings).
2c. In Advanced settings remove "admin/*" from the list of pages that should not load the colorbox script.
2d. Save the colorbox settings.

3. Activate all the Rooms modules and their dependencies. (dependencies
will be automatically downloaded if using drush to enable the Rooms
modules.)

4. Visit admin/rooms/units/unit-types and create a unit type (e.g. standard double room).

5. Visit admin/rooms/units and create a couple of bookable units.

6. Set availability and pricing - clicking on events will bring up a colorbox and allow you to interact with them.

7. Create a Booking Type at admin/rooms/bookings/booking-types FIXME
7a. *** Note create at least one Booking Type called Basic (machine-name:basic) - this will be used by the Booking Manager later on
(this should eventually be automated)

8. Visit admin/commerce/customer-profiles and create a Customer Profile in Drupal Commerce.

9. Create a Booking at admin/rooms/bookings/add

10. Go to /booking and do an availability search.

11. If you wish to display availability information on an embedded calendar in a node (as vacation rentals properties often do) activate the Rooms Availability Reference Module.
11a. This will give you a new field type called "Availability Reference".
11b. Add the field to any entity and you may reference specific Booking Units to have the availability information rendered on the node display (or any other entity).


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
