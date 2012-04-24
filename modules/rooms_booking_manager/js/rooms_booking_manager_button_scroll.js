(function ($) {
  $(document).ready(function () {
    var top = $('#rooms_booking_button').offset().top - parseFloat($('#rooms_booking_button').css('marginTop').replace(/auto/,0));
    
    $(window).scroll(function () {
      // let's do something funky
    });
  });

  $(document).ready(function () {  
    var top = $('#rooms_booking_button').offset().top - parseFloat($('#rooms_booking_button').css('marginTop').replace(/auto/, 0));
    $(window).scroll(function (event) {
      // what the y position of the scroll is
      var y = $(this).scrollTop();
    
      // whether that's below the form
      if (y >= top) {
        // if so, ad the fixed class
        $('#rooms_booking_button').addClass('fixed');
      } else {
        // otherwise remove it
        $('#rooms_booking_button').removeClass('fixed');
      }
    });
  });
})(jQuery);