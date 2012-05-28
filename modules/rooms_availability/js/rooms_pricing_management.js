(function ($) {

$(document).ready(function()
{
  $("form#rooms-availability-filter-month-form select").change(function() {
    $("form#rooms-availability-filter-month-form").submit();
  });
});

Drupal.behaviors.rooms_availability = {
  attach: function(context) {
        
    // Current month is whatever comes through -1 since js counts months starting
    // from 0
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
    
    var c = 0;
    $.each(calendars, function(key, value) {
      phpmonth = value[1]+1;
      $(value[0]).once().fullCalendar({
        editable:false,
        defaultView:'singleRowMonth',
        month:value[1],
        year:value[2],
        header:{
          left: 'title',
          center: '',
          right: ''
        },
        events: Drupal.settings.basePath + '?q=admin/rooms/units/unit/' + Drupal.settings.roomsUnitManagement.roomsId[c] + '/pricing/json/' + value[2] + '/' + phpmonth
      });

      c++;
    });
    
    // Resize takes care of some quirks on occasion
    $(window).resize();

  }
};
})(jQuery);