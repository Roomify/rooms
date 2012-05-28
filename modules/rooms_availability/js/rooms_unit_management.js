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
      // phpmonth is what we send via the url and need to add one since php handles
      // months starting from 1 not zero
      phpmonth = value[1]+1;
      $(value[0]).once().fullCalendar({
        ignoreTimezone:false,
        editable:false,
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
          date = $.fullCalendar.parseDate(calEvent.start)
          var sd = Math.round(Date.parse(calEvent.start)/1000);
          var ed = Math.round(Date.parse(calEvent.end)/1000);
          if ($.colorbox) {
            
            var url = Drupal.settings.basePath + '?q=admin/rooms/units/unit/' + Drupal.settings.roomsUnitManagement.roomsId[c] + '/event/' + calEvent.id + '/' + sd + '/' + ed; 
            $.colorbox({
              href: url,
              opacity:0.7,
              width: 400,
              height: 400,
              onClosed:function(){
                $(value[0]).fullCalendar('refetchEvents');
              }
            });
          }
        }

      });

      c++;
    });
    
    // Resize takes care of some quirks on occasion
    $(window).resize();

  }
};
})(jQuery);