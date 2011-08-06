INTRODUCTION
------------
Rooms is a family of modules that enable you to manage Booking for hotels, b&bs
and vacation rentals.

It builds on the Drupal CMS (drupal.org) and its modules.

To try out Rooms in this early stage do the following:

1. Download the FullCalendar library from http://arshaw.com/fullcalendar/downloads/fullcalendar-1.5.1.zip
and place in sites/all/libraries so that you end up with sites/all/libraries/fullcalendar/fullcalendar

2. Install the colorbox module - http://drupal.org/project/colorbox

2a. Follow the colobox module instructions to install the module and colorbox library
2b. Go to admin/config/media/colorbox and enable Colorbox load (under extra settings)
2c. In Advanced settings remove the admin* line from the set of pages that should not load the coloborx script

3. Activate all the Room modules and their dependencies.

4. Visit admin/rooms/unit-types and create a unit type (e.g. standard double room)

5. Visit admin/rooms/units and create a couple of bookable units

6. Set availability and pricing - clicking on events will bring up a colorbox and allows you to interact with it

7. Create a Booking Type at admin/rooms/booking-types
7a. *** Note create at least one Booking Type called Basic (machine-name:basic) - this will be used by the Booking Manager later on
(this should eventually be automated)

8. Create a Customer Profile in Commerce

9. Create a Booking at admin/rooms/bookings/add

10. Go to /booking and do an availability search

11. If you want to embed availability information on a calendar in a node (like vacation rentals often do) activate the Rooms Availability Reference Module
11a. This will give you a new field type "Availability Reference"
11b. Add the field to any entity and you can point to Booking Units and have the availability information rendered in the node (or any other entity)


The results of the availability search are the rooms available over the period - adding one to a cart
will create a Room Booking Unit commerce product for it and place it in the cart. Once checkout is completed
the calendar is updated to reflect the change.

For the rest join us in the issue queues for now and work with us to create a great
booking solution for Drupal!

PHP REQUIREMENTS & DATE REQUIREMENTS
--------------------------------------
The use of the DateInterval class means that we need at least PHP5.3

In general this module makes use of latest Date functions in PHP so make sure those are
enabled in your PHP setup. We may scale back on this to make things more usable for a wider range of
setups.