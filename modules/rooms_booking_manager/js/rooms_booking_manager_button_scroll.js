(function ($) {
  $(document).ready(function () {
    if ($('.booking_legend').offset() != null) {
      var top = $('.booking_legend').offset().top - parseFloat($('.booking_legend').css('marginTop').replace(/auto/,0));
    }

    $(window).scroll(function () {
      // let's do something funky
    });
  });

  $(document).ready(function () {
    if ($('.booking_legend').offset() != null) {
      var top = $('.booking_legend').offset().top - parseFloat($('.booking_legend').css('marginTop').replace(/auto/, 0));
      $(window).scroll(function (event) {
        // what the y position of the scroll is
        var y = $(this).scrollTop();

        // whether that's below the form
        if (y >= top) {
          // if so, ad the fixed class
          $('#rooms_booking_button').addClass('fixed');
          $('.booking_legend').addClass('fixed');
        } else {
          // otherwise remove it
          $('#rooms_booking_button').removeClass('fixed');
          $('.booking_legend').removeClass('fixed');
        }
      });
    }
  });
})(jQuery);
