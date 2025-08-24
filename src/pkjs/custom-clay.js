module.exports = function(minified) {
  var clayConfig = this;
  var _ = minified._;
  var $ = minified.$;
  var HTML = minified.HTML;

  const DEFAULT_VALUES = [["FFFFFF", "AA5500", "000000", "FF0000", "000000"], // Beagle
    ["FFFFFF", "AA0000", "000000", "FFAA00", "000000"], // Cow
    ["FF5500", "FFFFFF", "000000", "000000", "000000"], // Fox
    ["FFFFFF", "000000", "000000", "FFAA00", "000000"], // Goat
    ["FFFFAA", "FFAAAA", "000000", "000000", "000000"], // Jerboa
    ["AA5555", "550000", "000000", "000000", "000000"], // Pony
    ["0055FF", "FFFFFF", "FFAAAA", "000000", "000000"], // Shark
    ["AAAAAA", "555555", "000000", "000000", "000000"], // Tabby
    ["FFFFFF", "00AAFF", "000000", "FFAA00", "000000"], // Terrier
    ["FFFFFF", "AAAAAA", "FFAAAA", "FFAAAA", "000000"], // Bunny
    ["FFAAAA", "000000", "FF5555", "FF0000", "000000"], // Axolotl
    ["0055AA", "00AAFF", "000000", "000000", "000000"], // Lobster
    ["FFFFAA", "AA5500", "000000", "550000", "000000"], // Hedgehog
    ["AAFF00", "005500", "000000", "AAAA55", "000000"]]; // Turtle

  const DEFAULT_VALUES_BW = [["FFFFFF", "555555", "555555", "555555", "000000"], // Beagle
    ["FFFFFF", "555555", "000000", "555555", "000000"], // Cow
    ["555555", "FFFFFF", "555555", "FFFFFF", "000000"], // Fox
    ["FFFFFF", "000000", "000000", "555555", "000000"], // Goat
    ["FFFFFF", "555555", "000000", "000000", "000000"], // Jerboa
    ["FFFFFF", "555555", "000000", "000000", "000000"], // Pony
    ["555555", "FFFFFF", "555555", "000000", "000000"], // Shark
    ["555555", "FFFFFF", "000000", "000000", "000000"], // Tabby
    ["FFFFFF", "555555", "555555", "555555", "000000"], // Terrier
    ["FFFFFF", "555555", "555555", "555555", "000000"], // Bunny
    ["FFFFFF", "000000", "555555", "555555", "000000"], // Axolotl
    ["555555", "FFFFFF", "000000", "000000", "FFFFFF"], // Lobster
    ["FFFFFF", "555555", "555555", "555555", "000000"], // Hedgehog
    ["FFFFFF", "555555", "000000", "FFFFFF", "000000"]]; // Turtle

  function get_default(creature, key) {
    if (!clayConfig.meta.activeWatchInfo || clayConfig.meta.activeWatchInfo.platform === 'aplite' || clayConfig.meta.activeWatchInfo.platform === 'diorite')
      return DEFAULT_VALUES_BW[Number(creature)][Number(key) - 1];

    return DEFAULT_VALUES[Number(creature)][Number(key) - 1];
  }

  function fetchSettings() {
    var creature = this.get();
    clayConfig.getItemByMessageKey('ColorOne').set(get_default(creature, 1));
    clayConfig.getItemByMessageKey('ColorTwo').set(get_default(creature, 2));
    clayConfig.getItemByMessageKey('ColorThree').set(get_default(creature, 3));
    clayConfig.getItemByMessageKey('ColorFour').set(get_default(creature, 4));
    clayConfig.getItemByMessageKey('ColorFive').set(get_default(creature, 5));
  }

  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {
    var creatureToggle = clayConfig.getItemByMessageKey('Creature');
    creatureToggle.on('change', fetchSettings);
  });
  
};
