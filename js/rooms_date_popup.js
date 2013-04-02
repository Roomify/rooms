(function ($) {

Drupal.behaviors.rooms_date_popup = {
  attach: function(context) {
    var dates = $( "#edit-rooms-start-date-datepicker-popup-0, #edit-rooms-end-date-datepicker-popup-0" ).datepicker({
    dateFormat: Drupal.settings.rooms.roomsDateFormat,
      //defaultDate: "+1w",
      minDate: "+" + Drupal.settings.rooms.roomsBookingStartDay + "d",
      //maxDate: "+2M +10D", // This can be set via a variable to somthing reasonable
      //changeMonth: true,
      //numberOfMonths: 1,
      onSelect: function( selectedDate ) {
        var option = this.id == "edit-rooms-start-date-datepicker-popup-0" ? "minDate" : "na", //can change na to maxDate to set the range
          instance = $( this ).data( "datepicker" ),
          date = $.datepicker.parseDate(
            instance.settings.dateFormat ||
            $.datepicker._defaults.dateFormat,
            selectedDate, instance.settings );
        dates.not( this ).datepicker( "option", option, date );
      },
      beforeShow: function()
      {
        setTimeout(function()
          {
            // If you think this is ugly you are right - read this though: http://blog.foersom.dk/post/598839422/dealing-with-z-index-in-jquery-uis-datepicker
            $(".ui-datepicker").css("z-index", 12);
          }, 10);
      }
    });
  }
};
})(jQuery);
