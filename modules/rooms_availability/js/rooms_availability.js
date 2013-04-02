(function ($) {
// define object
Drupal.RoomsAvailability = Drupal.RoomsAvailability || {};
Drupal.RoomsAvailability.Modal = Drupal.RoomsAvailability.Modal || {};

Drupal.behaviors.rooms_availability = {
  attach: function(context) {

    // Current month is whatever comes through -1 since js counts months starting from 0
    currentmonth = parseInt(Drupal.settings.roomsAvailability.currentMonth)-1;
    currentyear = parseInt(Drupal.settings.roomsAvailability.currentYear);

    // The first month on the calendar
    month1 = currentmonth;
    year1 = currentyear;

    // Second month is the next one obviously unless it is 11 in which case we need to move a year ahead
    if (currentmonth == 11) {
      month2 = 0;
      year2 = year1 + 1;
    }
    else{
      month2 = currentmonth+1;
      year2 = currentyear;
    }

    currentmonth = month2;
    // And finally the last month where we do the same as above worth streamlining this probably
    if (currentmonth == 11) {
      month3 = 0;
      year3 = year2 + 1;
    }
    else{
      month3 = currentmonth+1;
      year3 = year2;
    }

    var calendars = new Array();
    calendars[0] = new Array('#calendar', month1, year1);
    calendars[1] = new Array('#calendar1', month2, year2);
    calendars[2] = new Array('#calendar2', month3, year3);

    // refresh the events once the modal is closed
    $("#modalContent a.close").once().bind('click', function(e) {
      $.each(calendars, function(key, value) {
        $(value[0]).fullCalendar('refetchEvents');
      });
    });

    $.each(calendars, function(key, value) {
      // phpmonth is what we send via the url and need to add one since php handles
      // months starting from 1 not zero
      phpmonth = value[1]+1;
      $(value[0]).once().fullCalendar({
        ignoreTimezone: false,
        editable: false,
        selectable: true,
        month: value[1],
        year: value[2],
        header:{
          left: 'title',
          center: '',
          right: ''
        },
        events: Drupal.settings.basePath + '?q=rooms/units/unit/' + Drupal.settings.roomsAvailability.roomID + '/availability/json/' + value[2] + '/' + phpmonth,
        eventClick: function(calEvent, jsEvent, view) {
          // Getting the Unix timestamp - JS will only give us milliseconds
          if (calEvent.end == null) {
            //We are probably dealing with a single day event
            calEvent.end = calEvent.start;
          }
          var localOffset = (-1) * calEvent.start.getTimezoneOffset() * 60000;
          var sd = Math.round((calEvent.start.getTime()+localOffset)/1000);
          var ed = Math.round((calEvent.end.getTime()+localOffset)/1000);
          // Open the modal for edit
          Drupal.RoomsAvailability.Modal(view, calEvent.id, sd, ed);
        },
        select: function(start, end, allDay) {
          var localOffset = (-1) * start.getTimezoneOffset() * 60000;
          var sd = Math.round((start.getTime()+localOffset)/1000);
          var ed = Math.round((end.getTime()+localOffset)/1000);
          // Open the modal for edit
          Drupal.RoomsAvailability.Modal(this, -2, sd, ed);
          $(value[0]).fullCalendar('unselect');
        }
      });
    });
  }
};

/**
* Initialize the modal box.
*/
Drupal.RoomsAvailability.Modal = function(element, eid, sd, ed) {
  // prepare the modal show with the rooms-availability settings.
  Drupal.CTools.Modal.show('rooms-modal-style');
  // base url the part that never change is used to identify our ajax instance
  var base = Drupal.settings.basePath + '?q=admin/rooms/units/unit/';
  // Create a drupal ajax object that points to the rooms availability form.
  var element_settings = {
    url : base + Drupal.settings.roomsAvailability.roomID + '/event/' + eid + '/' + sd + '/' + ed,
    event : 'getResponse',
    progress : { type: 'throbber' },
  };
  // To made all calendars trigger correctly the getResponse event we need to
  // initialize the ajax instance with the global calendar table element.
  var calendars_table = $(element.element).closest('table');
  // create new instance only once if exists just override the url
  if (Drupal.ajax[base] === undefined) {
    Drupal.ajax[base] = new Drupal.ajax(element_settings.url, calendars_table, element_settings);
  }
  else {
    Drupal.ajax[base].element_settings.url = element_settings.url;
    Drupal.ajax[base].options.url = element_settings.url;
  }
  // We need to trigger manually the AJAX getResponse due fullcalendar select
  // event is not recognized by Drupal AJAX
  $(calendars_table).trigger('getResponse');
};

})(jQuery);
