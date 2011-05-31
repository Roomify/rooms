(function ($) {

Drupal.behaviors.rooms_availability = {
  attach: function(context) {
    
    $('#calendar').fullCalendar({
      editable:true,
      events: Drupal.settings.basePath + 'admin/rooms/rooms/room/ajax/test'
    });
    
    alert(Drupal.settings.roomId);
  }
};
})(jQuery);
  
  

