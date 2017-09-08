$(document).ready(function(){
    $('.parallax').parallax();
    $('.materialboxed').materialbox();
    $(".button-collapse").sideNav();
    $('.slider').slider();
});

jQuery(function() {
    function setAspectRatio() {
        jQuery('iframe').each(function() {
            jQuery(this).css('height', jQuery(this).width() * 9/16);
        });
    }

    setAspectRatio();
    jQuery(window).resize(setAspectRatio);
});