(function ($) {

Drupal.behaviors.rooms_availability = {
  attach: function(context) {
        
    // Current month is whatever comes through -1 since js counts months starting
    // from 0
    currentmonth = parseInt(Drupal.settings.roomsAvailability.currentMonth)-1;
    currentyear = parseInt(Drupal.settings.roomsAvailability.currentYear);
    
    // The first month on the calendar
    month1 = currentmonth;
    year1 = currentyear;
    
    // Second month is the next one obviously unless it is 11 in
    // which case we need to move a year ahead 
    if (currentmonth == 11) {
      month2 = 0;
      year2 = year1 + 1;
    }
    else{
      month2 = currentmonth+1;
      year2 = currentyear;
    }

    currentmonth = month2;
    // And finally the last month where we do the same as above
    // worth streamlining this probably
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
    
    
    $.each(calendars, function(key, value) {
      // phpmonth is what we send via the url and need to add one since php handles
      // months starting from 1 not zero
      phpmonth = value[1]+1;
      $(value[0]).once().fullCalendar({
        ignoreTimezone:false,
        editable:false,
        month:value[1],
        year:value[2],
        header:{
          left: 'title',
          center: '',
          right: ''
        },
        events: Drupal.settings.basePath + '?q=admin/rooms/units/unit/' + Drupal.settings.roomsAvailability.roomID + '/availability/json/' + value[2] + '/' + phpmonth,
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
            
            var url = Drupal.settings.basePath + '?q=admin/rooms/units/unit/' + Drupal.settings.roomsAvailability.roomID + '/event/' + calEvent.id + '/' + sd + '/' + ed; 
            $.colorbox({
              href: url,
              opacity:0.7,
              width: 400,
              height: 400,
              onClosed:function(){
                $(value[0]).fullCalendar('refetchEvents');  
                //$('#calendar').fullCalendar('rerender');
              }
            });   
          }
        }
//        eventMouseover : function(event, jsEvent, view) {
//          $(value[0]).css('border', '10px solid red');
//          //event.color = 'yellow';
//          $(value[0]).fullCalendar('updateEvent', event);
//        },
//        eventMouseout : function(event, jsEvent, view) {
//          //$(this).css('border', '10px solid red');
//          event.title = 'red';
//          $(value[0]).fullCalendar('updateEvent', event);
//        }
      });
    });
    
    // Resize takes care of some quirks on occasion
    $(window).resize();

  }
};
})(jQuery);
  
  

