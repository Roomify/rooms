(function ($) {

Drupal.behaviors.rooms_availability = {
  attach: function(context) {
        
    currentmonth = parseInt(Drupal.settings.currentMonth);
    currentyear = parseInt(Drupal.settings.currentYear);
    if (currentmonth == 12) {
      nextmonth1 = 1;
      nextyear1 = parseInt(Drupal.settings.currentYear) + 1;
      nextmonth2 = 2;
      nextyear2 = parseInt(Drupal.settings.currentYear) + 1;
    }
    if (currentmonth == 11){
      nextmonth1 = 12;
      nextyear1 = Drupal.settings.currentYear;
      nextmonth2 = 2;
      nextyear2 = parseInt(Drupal.settings.currentYear) + 1;
    }
    else{
      nextmonth1 = currentmonth+1;
      nextyear1 = currentyear;
      nextmonth2 = currentmonth+2;
      nextyear2 = currentyear;
    }

    var calendars = new Array();
    calendars[0] = new Array('#calendar', currentmonth, currentyear);
    calendars[1] = new Array('#calendar1', nextmonth1, nextyear1);
    calendars[2] = new Array('#calendar2', nextmonth2, nextyear2);
 
    $.each(calendars, function(key, value) {
      $(value[0]).once().fullCalendar({
        editable:false,
        month:value[1]-1,
        year:Drupal.settings.currentYear,
        header:{
          left: 'title',
          center: '',
          right: ''
        },
        events: Drupal.settings.basePath + 'admin/rooms/units/unit/' + Drupal.settings.roomID + '/pricing/json/' + value[2] + '/' + value[1]
      });
    });  
    // Resize takes care of some quirks on occasion
    $(window).resize();
  }
};
})(jQuery);
  
  

