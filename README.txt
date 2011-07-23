INTRODUCTION
------------
Rooms is a family of modules that enable you to manage Booking for hotels, b&bs
and vacation rentals.

It builds on the Drupal CMS (drupal.org) and its modules.


To try out Rooms in this early stage do the following:

1. Download the FullCalendar library from http://arshaw.com/fullcalendar/downloads/fullcalendar-1.5.1.zip
and place in sites/all/libraries so that you end up with sites/all/libraries/fullcalendar

2. Set up the colorbox module
2a. Go to admin/config/media/colorbox and enable Colorbox load
2b. In Advanced settings remove the admin* line from the set of pages that should not load the coloborx script

3. Activate all the Room modules and their dependencies.

4. Visit admin/rooms/unit-types and create a unit type (e.g. standard double room)

5. Visit admin/rooms/units and create a couple of bookable units

6. Set availability and pricing - clicking on events will bring up a colorbox and allow you to interact with it

7. Got to /booking and do an availability search

The results of the availability search are the rooms available over the period - adding one to a cart
will create a Room Booking Unit commerce product for it and place it in the cart. Once checkout is completed
the calendar is updated to reflect the change.

For the rest join us in the issue queues for now and work with us to create a great
booking solution for Drupal!

PHP REQUIREMENTS
----------------
The use of the DateInterval class means that we need at least PHP5.3