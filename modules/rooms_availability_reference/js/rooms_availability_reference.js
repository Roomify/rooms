(function ($) {

Drupal.behaviors.rooms_availability_reference = {
  attach: function(context) {    
    var minDate =new Date();
    $('#calendar').fullCalendar({
      ignoreTimezone:false,
      editable:false,
      month:minDate.getMonth(),
      year:minDate.getFullYear(),
      header:{
        left: 'today',
				center: 'title',
				right: 'prev, next'
      },
      events: Drupal.settings.basePath + '?q=rooms/units/unit/'
                                        + Drupal.settings.roomsAvailabilityRef.unitID
                                        + '/availability/json/'
                                        + Drupal.settings.roomsAvailabilityRef.startyear
                                        + '/'
                                        + Drupal.settings.roomsAvailabilityRef.startmonth
                                        +'/1/' //start day
                                        + Drupal.settings.roomsAvailabilityRef.endyear
                                        +'/'
                                        + Drupal.settings.roomsAvailabilityRef.endmonth
                                        +'/'
                                        + Drupal.settings.roomsAvailabilityRef.endday
                                        + '/'
                                        + Drupal.settings.roomsAvailabilityRef.style
    });
    
    
    
    // Resize takes care of some quirks on occasion
    $(window).resize();

  }
};
})(jQuery);
  
  

