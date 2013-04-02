(function ($) {
// define object
Drupal.RoomsAvailability = Drupal.RoomsAvailability || {};
Drupal.RoomsAvailability.Modal = Drupal.RoomsAvailability.Modal || {};

$(document).ready(function()
{
  $("form#rooms-availability-filter-month-form select").change(function() {
    $("form#rooms-availability-filter-month-form").submit();
  });

  $('#edit-select-all').change(function() {
    if (this.options.selectedIndex == 1) {
      var table = $(this).closest('table')[0];
      $('input[id^="edit-rooms"]:not(:disabled)', table).attr('checked', true);

      jQuery.ajax({
        type: 'POST',
        url: Drupal.settings.basePath + '?q=admin/rooms/select-all-pages-av',
        data: {'select-all': '0'},
      });
    }
    else if (this.options.selectedIndex == 2) {
      jQuery.ajax({
        type: 'POST',
        url: Drupal.settings.basePath + '?q=admin/rooms/select-all-pages-av',
        data: {'select-all': '1'},
      });

      var table = $(this).closest('table')[0];
      $('input[id^="edit-rooms"]:not(:disabled)', table).attr('checked', true);
    }
    else if (this.options.selectedIndex == 3) {
      var table = $(this).closest('table')[0];
      $('input[id^="edit-rooms"]:not(:disabled)', table).attr('checked', false);

      jQuery.ajax({
        type: 'POST',
        url: Drupal.settings.basePath + '?q=admin/rooms/select-all-pages-av',
        data: {'select-all': '0'},
      });
    }
  });
});

Drupal.behaviors.rooms_availability = {
  attach: function(context) {

    // Current month is whatever comes through -1 since js counts months starting from 0
    currentmonth = parseInt(Drupal.settings.roomsUnitManagement.currentMonth)-1;
    currentyear = parseInt(Drupal.settings.roomsUnitManagement.currentYear);

    // The first month on the calendar
    month1 = currentmonth;
    year1 = currentyear;

    var calendars = new Array();
    var i = 0;
    for (i=0;i<Drupal.settings.roomsUnitManagement.roomsNumber;i++) {
      calendars[i] = new Array('#calendar' + i, month1, year1);
    }

    // refresh the events once the modal is closed
    $("#modalContent a.close").once().bind('click', function(e) {
      $.each(calendars, function(key, value) {
        $(value[0]).fullCalendar('refetchEvents');
      });
    });

    var c = 0;
    $.each(calendars, function(key, value) {
      // phpmonth is what we send via the url and need to add one since php handles
      // months starting from 1 not zero
      phpmonth = value[1]+1;

      var unit_id = Drupal.settings.roomsUnitManagement.roomsId[c];

      $(value[0]).once().fullCalendar({
        ignoreTimezone:false,
        editable:false,
        selectable: true,
        defaultView:'singleRowMonth',
        month:value[1],
        year:value[2],
        header:{
          left: 'title',
          center: '',
          right: ''
        },
        events: Drupal.settings.basePath + '?q=rooms/units/unit/' + Drupal.settings.roomsUnitManagement.roomsId[c] + '/availability/json/' + value[2] + '/' + phpmonth,
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
          Drupal.RoomsAvailability.Modal(view, unit_id, calEvent.id, sd, ed);
        },
        select: function(start, end, allDay) {
          var localOffset = (-1) * start.getTimezoneOffset() * 60000;
          var sd = Math.round((start.getTime()+localOffset)/1000);
          var ed = Math.round((end.getTime()+localOffset)/1000);
          // Open the modal for edit
          Drupal.RoomsAvailability.Modal(this, unit_id, -2, sd, ed);
          $(value[0]).fullCalendar('unselect');
        }

      });

      c++;
    });

    // Resize takes care of some quirks on occasion
    $(window).resize();

  }
};

/**
* Initialize the modal box.
*/
Drupal.RoomsAvailability.Modal = function(element, unit_id, eid, sd, ed) {
  // prepare the modal show with the rooms-availability settings.
  Drupal.CTools.Modal.show('rooms-modal-style');
  // base url the part that never change is used to identify our ajax instance
  var base = Drupal.settings.basePath + '?q=admin/rooms/units/unit/';
  // Create a drupal ajax object that points to the rooms availability form.
  var element_settings = {
    url : base + unit_id + '/event/' + eid + '/' + sd + '/' + ed,
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
