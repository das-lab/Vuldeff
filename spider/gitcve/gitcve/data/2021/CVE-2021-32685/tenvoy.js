const openpgp = require('../openpgpjs/dist/openpgp.js');
const nacl = require('../tweetnacljs/nacl.js');
const sha256 = require('../fast-sha256-js');

var TogaTech = {};

function tEnvoy(openpgpRef = openpgp, naclRef = nacl, sha256Ref = sha256) {
	let _openpgp = openpgpRef;
	let _nacl = naclRef;
	let _sha256 = sha256Ref;
	
	
	this.dictionary = "abandon ability able about above absent absorb abstract absurd abuse access accident account accuse achieve acid acoustic acquire across act action actor actress actual adapt add addict address adjust admit adult advance advice aerobic affair afford afraid again age agent agree ahead aim air airport aisle alarm album alcohol alert alien all alley allow almost alone alpha already also alter always amateur amazing among amount amused analyst anchor ancient anger angle angry animal ankle announce annual another answer antenna antique anxiety any apart apology appear apple approve april arch arctic area arena argue arm armed armor army around arrange arrest arrive arrow art artefact artist artwork ask aspect assault asset assist assume asthma athlete atom attack attend attitude attract auction audit august aunt author auto autumn average avocado avoid awake aware away awesome awful awkward axis baby bachelor bacon badge bag balance balcony ball bamboo banana banner bar barely bargain barrel base basic basket battle beach bean beauty because become beef before begin behave behind believe below belt bench benefit best betray better between beyond bicycle bid bike bind biology bird birth bitter black blade blame blanket blast bleak bless blind blood blossom blouse blue blur blush board boat body boil bomb bone bonus book boost border boring borrow boss bottom bounce box boy bracket brain brand brass brave bread breeze brick bridge brief bright bring brisk broccoli broken bronze broom brother brown brush bubble buddy budget buffalo build bulb bulk bullet bundle bunker burden burger burst bus business busy butter buyer buzz cabbage cabin cable cactus cage cake call calm camera camp can canal cancel candy cannon canoe canvas canyon capable capital captain car carbon card cargo carpet carry cart case cash casino castle casual cat catalog catch category cattle caught cause caution cave ceiling celery cement census century cereal certain chair chalk champion change chaos chapter charge chase chat cheap check cheese chef cherry chest chicken chief child chimney choice choose chronic chuckle chunk churn cigar cinnamon circle citizen city civil claim clap clarify claw clay clean clerk clever click client cliff climb clinic clip clock clog close cloth cloud clown club clump cluster clutch coach coast coconut code coffee coil coin collect color column combine come comfort comic common company concert conduct confirm congress connect consider control convince cook cool copper copy coral core corn correct cost cotton couch country couple course cousin cover coyote crack cradle craft cram crane crash crater crawl crazy cream credit creek crew cricket crime crisp critic crop cross crouch crowd crucial cruel cruise crumble crunch crush cry crystal cube culture cup cupboard curious current curtain curve cushion custom cute cycle dad damage damp dance danger daring dash daughter dawn day deal debate debris decade december decide decline decorate decrease deer defense define defy degree delay deliver demand demise denial dentist deny depart depend deposit depth deputy derive describe desert design desk despair destroy detail detect develop device devote diagram dial diamond diary dice diesel diet differ digital dignity dilemma dinner dinosaur direct dirt disagree discover disease dish dismiss disorder display distance divert divide divorce dizzy doctor document dog doll dolphin domain donate donkey donor door dose double dove draft dragon drama drastic draw dream dress drift drill drink drip drive drop drum dry duck dumb dune during dust dutch duty dwarf dynamic eager eagle early earn earth easily east easy echo ecology economy edge edit educate effort egg eight either elbow elder electric elegant element elephant elevator elite else embark embody embrace emerge emotion employ empower empty enable enact end endless endorse enemy energy enforce engage engine enhance enjoy enlist enough enrich enroll ensure enter entire entry envelope episode equal equip era erase erode erosion error erupt escape essay essence estate eternal ethics evidence evil evoke evolve exact example excess exchange excite exclude excuse execute exercise exhaust exhibit exile exist exit exotic expand expect expire explain expose express extend extra eye eyebrow fabric face faculty fade faint faith fall false fame family famous fan fancy fantasy farm fashion fat fatal father fatigue fault favorite feature february federal fee feed feel female fence festival fetch fever few fiber fiction field figure file film filter final find fine finger finish fire firm first fiscal fish fit fitness fix flag flame flash flat flavor flee flight flip float flock floor flower fluid flush fly foam focus fog foil fold follow food foot force forest forget fork fortune forum forward fossil foster found fox fragile frame frequent fresh friend fringe frog front frost frown frozen fruit fuel fun funny furnace fury future gadget gain galaxy gallery game gap garage garbage garden garlic garment gas gasp gate gather gauge gaze general genius genre gentle genuine gesture ghost giant gift giggle ginger giraffe girl give glad glance glare glass glide glimpse globe gloom glory glove glow glue goat goddess gold good goose gorilla gospel gossip govern gown grab grace grain grant grape grass gravity great green grid grief grit grocery group grow grunt guard guess guide guilt guitar gun gym habit hair half hammer hamster hand happy harbor hard harsh harvest hat have hawk hazard head health heart heavy hedgehog height hello helmet help hen hero hidden high hill hint hip hire history hobby hockey hold hole holiday hollow home honey hood hope horn horror horse hospital host hotel hour hover hub huge human humble humor hundred hungry hunt hurdle hurry hurt husband hybrid ice icon idea identify idle ignore ill illegal illness image imitate immense immune impact impose improve impulse inch include income increase index indicate indoor industry infant inflict inform inhale inherit initial inject injury inmate inner innocent input inquiry insane insect inside inspire install intact interest into invest invite involve iron island isolate issue item ivory jacket jaguar jar jazz jealous jeans jelly jewel job join joke journey joy judge juice jump jungle junior junk just kangaroo keen keep ketchup key kick kid kidney kind kingdom kiss kit kitchen kite kitten kiwi knee knife knock know lab label labor ladder lady lake lamp language laptop large later latin laugh laundry lava law lawn lawsuit layer lazy leader leaf learn leave lecture left leg legal legend leisure lemon lend length lens leopard lesson letter level liar liberty library license life lift light like limb limit link lion liquid list little live lizard load loan lobster local lock logic lonely long loop lottery loud lounge love loyal lucky luggage lumber lunar lunch luxury lyrics machine mad magic magnet maid mail main major make mammal man manage mandate mango mansion manual maple marble march margin marine market marriage mask mass master match material math matrix matter maximum maze meadow mean measure meat mechanic medal media melody melt member memory mention menu mercy merge merit merry mesh message metal method middle midnight milk million mimic mind minimum minor minute miracle mirror misery miss mistake mix mixed mixture mobile model modify mom moment monitor monkey monster month moon moral more morning mosquito mother motion motor mountain mouse move movie much muffin mule multiply muscle museum mushroom music must mutual myself mystery myth naive name napkin narrow nasty nation nature near neck need negative neglect neither nephew nerve nest net network neutral never news next nice night noble noise nominee noodle normal north nose notable note nothing notice novel now nuclear number nurse nut oak obey object oblige obscure observe obtain obvious occur ocean october odor off offer office often oil okay old olive olympic omit once one onion online only open opera opinion oppose option orange orbit orchard order ordinary organ orient original orphan ostrich other outdoor outer output outside oval oven over own owner oxygen oyster ozone pact paddle page pair palace palm panda panel panic panther paper parade parent park parrot party pass patch path patient patrol pattern pause pave payment peace peanut pear peasant pelican pen penalty pencil people pepper perfect permit person pet phone photo phrase physical piano picnic picture piece pig pigeon pill pilot pink pioneer pipe pistol pitch pizza place planet plastic plate play please pledge pluck plug plunge poem poet point polar pole police pond pony pool popular portion position possible post potato pottery poverty powder power practice praise predict prefer prepare present pretty prevent price pride primary print priority prison private prize problem process produce profit program project promote proof property prosper protect proud provide public pudding pull pulp pulse pumpkin punch pupil puppy purchase purity purpose purse push put puzzle pyramid quality quantum quarter question quick quit quiz quote rabbit raccoon race rack radar radio rail rain raise rally ramp ranch random range rapid rare rate rather raven raw razor ready real reason rebel rebuild recall receive recipe record recycle reduce reflect reform refuse region regret regular reject relax release relief rely remain remember remind remove render renew rent reopen repair repeat replace report require rescue resemble resist resource response result retire retreat return reunion reveal review reward rhythm rib ribbon rice rich ride ridge rifle right rigid ring riot ripple risk ritual rival river road roast robot robust rocket romance roof rookie room rose rotate rough round route royal rubber rude rug rule run runway rural sad saddle sadness safe sail salad salmon salon salt salute same sample sand satisfy satoshi sauce sausage save say scale scan scare scatter scene scheme school science scissors scorpion scout scrap screen script scrub sea search season seat second secret section security seed seek segment select sell seminar senior sense sentence series service session settle setup seven shadow shaft shallow share shed shell sheriff shield shift shine ship shiver shock shoe shoot shop short shoulder shove shrimp shrug shuffle shy sibling sick side siege sight sign silent silk silly silver similar simple since sing siren sister situate six size skate sketch ski skill skin skirt skull slab slam sleep slender slice slide slight slim slogan slot slow slush small smart smile smoke smooth snack snake snap sniff snow soap soccer social sock soda soft solar soldier solid solution solve someone song soon sorry sort soul sound soup source south space spare spatial spawn speak special speed spell spend sphere spice spider spike spin spirit split spoil sponsor spoon sport spot spray spread spring spy square squeeze squirrel stable stadium staff stage stairs stamp stand start state stay steak steel stem step stereo stick still sting stock stomach stone stool story stove strategy street strike strong struggle student stuff stumble style subject submit subway success such sudden suffer sugar suggest suit summer sun sunny sunset super supply supreme sure surface surge surprise surround survey suspect sustain swallow swamp swap swarm swear sweet swift swim swing switch sword symbol symptom syrup system table tackle tag tail talent talk tank tape target task taste tattoo taxi teach team tell ten tenant tennis tent term test text thank that theme then theory there they thing this thought three thrive throw thumb thunder ticket tide tiger tilt timber time tiny tip tired tissue title toast tobacco today toddler toe together toilet token tomato tomorrow tone tongue tonight tool tooth top topic topple torch tornado tortoise toss total tourist toward tower town toy track trade traffic tragic train transfer trap trash travel tray treat tree trend trial tribe trick trigger trim trip trophy trouble truck true truly trumpet trust truth try tube tuition tumble tuna tunnel turkey turn turtle twelve twenty twice twin twist two type typical ugly umbrella unable unaware uncle uncover under undo unfair unfold unhappy uniform unique unit universe unknown unlock until unusual unveil update upgrade uphold upon upper upset urban urge usage use used useful useless usual utility vacant vacuum vague valid valley valve van vanish vapor various vast vault vehicle velvet vendor venture venue verb verify version very vessel veteran viable vibrant vicious victory video view village vintage violin virtual virus visa visit visual vital vivid vocal voice void volcano volume vote voyage wage wagon wait walk wall walnut want warfare warm warrior wash wasp waste water wave way wealth weapon wear weasel weather web wedding weekend weird welcome west wet whale what wheat wheel when where whip whisper wide width wife wild will win window wine wing wink winner winter wire wisdom wise wish witness wolf woman wonder wood wool word work world worry worth wrap wreck wrestle wrist write wrong yard year yellow you young youth zebra zero zone zoo";
	this.wordsList = this.dictionary.split(" ");
	
	
	Object.defineProperty(this, "version", {
		get: () => {
			return "v7.0.0";
		}
	});
	
	this.core = {};
	
	Object.defineProperty(this.core, "openpgp", {
		get: () => {
			return _openpgp;
		}
	});
	Object.defineProperty(this.core, "nacl", {
		get: () => {
			return _nacl;
		}
	});
	Object.defineProperty(this.core, "sha256", {
		get: () => {
			return _sha256;
		}
	});
	
	
	this.util = {};
	
	this.util.utf8encode = (string) => {
		if(string == null) {
			throw "tEnvoy Fatal Error: argument string of method util.utf8encode is required and does not have a default value.";
		}
		return _openpgp.util.encode_utf8(string);
	}
	
	this.util.utf8decode = (bytes) => {
		if(bytes == null) {
			throw "tEnvoy Fatal Error: argument bytes of method util.utf8decode is required and does not have a default value.";
		}
		if(!(bytes instanceof Uint8Array)) {
			bytes = this.util.mixedToUint8Array(bytes, false);
		}
		return _openpgp.util.decode_utf8(bytes);
	}
	
	this.util.stringToBytes = (string) => {
		if(string == null) {
			throw "tEnvoy Fatal Error: argument string of method util.stringToBytes is required and does not have a default value.";
		}
		return _openpgp.util.str_to_Uint8Array(string);
	}
	
	this.util.bytesToString = (bytes) => {
		if(bytes == null) {
			throw "tEnvoy Fatal Error: argument bytes of method util.bytesToString is required and does not have a default value.";
		}
		if(!(bytes instanceof Uint8Array)) {
			bytes = this.util.mixedToUint8Array(bytes, false);
		}
		return _openpgp.util.Uint8Array_to_str(bytes);
	}
	
	this.util.stringToHex = (string) => {
		if(string == null) {
			throw "tEnvoy Fatal Error: argument string of method util.stringToHex is required and does not have a default value.";
		}
		let hex = "";
		for(let i = 0; i < string.length; i++) {
			let c = string.charCodeAt(i).toString(16);
			while(c.length < 2) {
				c = "0" + c;
			}
			hex += c;
		}
		return hex;
	}
	
	this.util.hexToString = (hex) => {
		if(hex == null) {
			throw "tEnvoy Fatal Error: argument hex of method util.hexToString is required and does not have a default value.";
		}
		let string = "";
		for(let i = 0; i < hex.length; i += 2) {
			string += String.fromCharCode(parseInt(hex.substring(i, i + 2), 16));
		}
		return string;
	}
	
	this.util.bytesToHex = (bytes) => {
		if(bytes == null) {
			throw "tEnvoy Fatal Error: argument bytes of method util.bytesToHex is required and does not have a default value.";
		}
		if(!(bytes instanceof Uint8Array)) {
			bytes = this.util.mixedToUint8Array(bytes, false);
		}
		let hex = "";
		for(let i = 0; i < bytes.length; i++) {
			if(bytes[i].toString(16).length == 0) {
				hex += "00";
			} else if(bytes[i].toString(16).length == 1) {
				hex += "0" + bytes[i].toString(16);
			} else {
				hex += bytes[i].toString(16);
			}
		}
		return hex;
	}
	
	this.util.hexToBytes = (hex) => {
		if(hex == null) {
			throw "tEnvoy Fatal Error: argument hex of method util.hexToBytes is required and does not have a default value.";
		}
		let bytes = new Uint8Array(hex.length / 2);
		for(let i = 0; i < hex.length; i += 2) {
			bytes[i / 2] = parseInt(hex.substring(i, i + 2), 16);
		}
		return bytes;
	}

	this.util.arrayDeepCopy = (array) => {
		if(array == null) {
			throw "tEnvoy Fatal Error: argument array of method util.arrayDeepCopy is required and does not have a default value.";
		}
		let copy;
		if(array instanceof Uint8Array) {
			copy = new Uint8Array(array.length);
		} else if(array instanceof Array) {
			copy = new Array(array.length);
		} else {
			throw "tEnvoy Fatal Error: argument array of method util.arrayDeepCopy is invalid, array must be of type Uint8Array or Array.";
		}
		for(let i = 0; i < array.length; i++) {
			copy[i] = array[i];
		}
		return copy;
	}

	this.util.compareConstant = (inputted, original) => {
		if(inputted == null) {
			throw "tEnvoy Fatal Error: argument inputted of method util.compareConstant is required and does not have a default value.";
		}
		if(original == null) {
			throw "tEnvoy Fatal Error: argument original of method util.compareConstant is required and does not have a default value.";
		}
		let result = true;
		for(let i = 0; i < inputted.length; i++) {
			if(i >= original.length || inputted[i] != original[i]) {
				result = false;
			}
		}
		if(inputted.length != original.length) {
			result = false;
		}
		return result;
	}
	
	this.util.mixedToUint8Array = (mixed, includeType = false, length = null) => {
		if(mixed == null) {
			return mixed;
		}
		
		let arrayOnlyContainsNumbers = (array) => {
			for(let i = 0; i < array.length; i++) {
				if(typeof array[i] != "number" || !Number.isInteger(array[i]) || array[i] < 0 || array[i] > 255) {
					return false;
				}
			}
			return true;
		}
		let pad = (array, length) => {
			if(array.length == length || length == null) {
				return array;
			} else if(array.length > length) {
				let returnArray = new Uint8Array(length);
				for(let i = 0; i < returnArray.length; i++) {
					returnArray[i] = array[i];
				}
				return returnArray;
			} else {
				let returnArray = new Uint8Array(length);
				for(let i = 0; i < returnArray.length; i++) {
					returnArray[i] = 255;
				}
				for(let i = 0; i < array.length; i++) {
					returnArray[returnArray.length - array.length + i] = array[i];
				}
				return returnArray;
			}
		}
		if(mixed == null) {
			throw "tEnvoy Fatal Error: argument mixed of method util.mixedToUint8Array is required and does not have a default value.";
		}
		if(mixed instanceof Uint8Array) {
			if(includeType) {
				let returnUint8Array = new Uint8Array(mixed.length + 1);
				returnUint8Array[0] = 0;
				for(let i = 0; i < mixed.length; i++) {
					returnUint8Array[i + 1] = mixed[i];
				}
				return pad(returnUint8Array, length);
			} else {
				return mixed;
			}
		} else if(mixed instanceof Array && arrayOnlyContainsNumbers(mixed)) {
			if(includeType) {
				let returnUint8Array = new Uint8Array(mixed.length + 1);
				returnUint8Array[0] = 1;
				for(let i = 0; i < mixed.length; i++) {
					returnUint8Array[i + 1] = mixed[i];
				}
				return pad(returnUint8Array, length);
			} else {
				let returnUint8Array = new Uint8Array(mixed.length);
				for(let i = 0; i < mixed.length; i++) {
					returnUint8Array[i] = mixed[i];
				}
				return returnUint8Array;
			}
		} else if(typeof mixed == "number") {
			if(Number.isInteger(mixed)) {
				if(mixed > 0) {
					let hex = mixed.toString(16);
					if(hex.length % 2 != 0) {
						hex = "0" + hex;
					}
					if(hex.length == 0) {
						hex = "00";
					}
					let hexAsArray = this.util.hexToBytes(hex);
					if(includeType) {
						let returnUint8Array = new Uint8Array(hexAsArray.length + 1);
						returnUint8Array[0] = 2;
						for(let i = 0; i < hexAsArray.length; i++) {
							returnUint8Array[i + 1] = hexAsArray[i];
						}
						return pad(returnUint8Array, length);
					} else {
						let returnUint8Array = new Uint8Array(hexAsArray.length);
						for(let i = 0; i < hexAsArray.length; i++) {
							returnUint8Array[i] = hexAsArray[i];
						}
						return returnUint8Array;
					}
				} else if(mixed < 0) {
					mixed = -mixed;
					let hex = mixed.toString(16);
					if(hex.length % 2 != 0) {
						hex = "0" + hex;
					}
					if(hex.length == 0) {
						hex = "00";
					}
					let hexAsArray = this.util.hexToBytes(hex);
					if(includeType) {
						let returnUint8Array = new Uint8Array(hexAsArray.length + 1);
						returnUint8Array[0] = 3;
						for(let i = 0; i < hexAsArray.length; i++) {
							returnUint8Array[i + 1] = hexAsArray[i];
						}
						return pad(returnUint8Array, length);
					} else {
						let returnUint8Array = new Uint8Array(hexAsArray.length);
						for(let i = 0; i < hexAsArray.length; i++) {
							returnUint8Array[i] = hexAsArray[i];
						}
						return returnUint8Array;
					}
				} else {
					if(includeType) {
						let returnUint8Array = new Uint8Array(2);
						returnUint8Array[0] = 4;
						returnUint8Array[1] = 0;
						return pad(returnUint8Array, length);
					} else {
						let returnUint8Array = new Uint8Array(1);
						returnUint8Array[0] = 0;
						return returnUint8Array;
					}
				}
			} else if(Number.isNaN(mixed)) {
				if(includeType) {
					let returnUint8Array = new Uint8Array(2);
					returnUint8Array[0] = 7;
					returnUint8Array[1] = 0;
					return pad(returnUint8Array, length);
				} else {
					let returnUint8Array = new Uint8Array(1);
					returnUint8Array[0] = 0;
					return returnUint8Array;
				}
			} else if(Number.isFinite(mixed)) {
				if(includeType) {
					let returnUint8Array = this.util.pack(mixed + "");
					returnUint8Array[0] = 8;
					return pad(returnUint8Array, length);
				} else {
					return this.util.mixedToUint8Array((mixed + ""), false);
				}
			} else {
				if(includeType) {
					let returnUint8Array = new Uint8Array(2);
					returnUint8Array[0] = 9;
					returnUint8Array[1] = 255;
					return pad(returnUint8Array, length);
				} else {
					let returnUint8Array = new Uint8Array(1);
					returnUint8Array[0] = 255;
					return returnUint8Array;
				}
			}
		} else if(mixed.constructor == Object || mixed.constructor == Array || typeof mixed.toJSON == "function") {
			let mixedAsUint8Array = this.util.utf8encode(JSON.stringify(mixed));
			if(includeType) {
				let returnUint8Array = new Uint8Array(mixedAsUint8Array.length + 1);
				returnUint8Array[0] = 5;
				for(let i = 0; i < mixedAsUint8Array.length; i++) {
					returnUint8Array[i + 1] = mixedAsUint8Array[i];
				}
				return pad(returnUint8Array, length);
			} else {
				return mixedAsUint8Array;
			}
		} else if(typeof mixed == "boolean") {
			if(includeType) {
				let returnUint8Array = new Uint8Array(2);
				returnUint8Array[0] = 6;
				returnUint8Array[1] = mixed ? 1 : 0;
				return pad(returnUint8Array, length);
			} else {
				let returnUint8Array = new Uint8Array(1);
				returnUint8Array[0] = mixed ? 1 : 0;
				return returnUint8Array;
			}
		} else {
			let mixedAsUint8Array = this.util.utf8encode(mixed.toString());
			if(includeType) {
				let returnUint8Array = new Uint8Array(mixedAsUint8Array.length + 1);
				returnUint8Array[0] = 254;
				for(let i = 0; i < mixedAsUint8Array.length; i++) {
					returnUint8Array[i + 1] = mixedAsUint8Array[i];
				}
				return pad(returnUint8Array, length);
			} else {
				return mixedAsUint8Array;
			}
		}
	}
	
	this.util.uint8ArrayToMixed = (uint8Array, includeType = false) => {
		if(uint8Array == null) {
			return null;
		}
		if(includeType) {
			let paddingOver = false;
			let startIndex = 0;
			for(let i = 0; i < uint8Array.length; i++) {
				if(uint8Array[i] != 255 && !paddingOver) {
					paddingOver = true;
					startIndex = i;
				}
			}
			let unpaddedUint8Array;
			let fakeUint8Array = new Uint8Array(startIndex);
			if(paddingOver) {
				unpaddedUint8Array = new Uint8Array(uint8Array.length - startIndex);
				for(let i = startIndex; i < uint8Array.length; i++) {
					unpaddedUint8Array[i - startIndex] = uint8Array[i];
				}
				if(_nacl != null) {
					fakeUint8Array = _nacl.randomBytes(startIndex);
				}
			} else {
				unpaddedUint8Array = uint8Array;
				if(_nacl != null) {
					fakeUint8Array = _nacl.randomBytes(_nacl.randomBytes(1)[0] % 16);
				}
			}
			uint8Array = unpaddedUint8Array;
			let returnUint8Array = new Uint8Array(uint8Array.length - 1);
			for(let i = 0; i < returnUint8Array.length; i++) {
				returnUint8Array[i] = uint8Array[i + 1];
			}
			if(uint8Array[0] == 1) {
				let returnArray = [];
				for(let i = 0; i < returnUint8Array.length; i++) {
					returnArray[i] = returnUint8Array[i];
				}
				let fakeReturnArray = [];
				for(let i = 0; i < fakeUint8Array.length; i++) {
					fakeReturnArray[i] = fakeUint8Array[i];
				}
				return returnArray;
			} else if(uint8Array[0] == 2) {
				let hex = this.util.bytesToHex(returnUint8Array);
				let fakeHex = this.util.bytesToHex(fakeUint8Array);
				return parseInt(hex, 16);
			} else if(uint8Array[0] == 3) {
				let hex = this.util.bytesToHex(returnUint8Array);
				let fakeHex = -1 * parseInt(this.util.bytesToHex(fakeUint8Array), 16);
				return -1 * parseInt(hex, 16);
			} else if(uint8Array[0] == 4) {
				return uint8Array[1];
			} else if(uint8Array[0] == 5) {
				let fakeRes = this.util.utf8decode(fakeUint8Array);
				let fakeJSON = JSON.parse("{}");
				return JSON.parse(this.util.utf8decode(returnUint8Array));
			} else if(uint8Array[0] == 6) {
				return returnUint8Array[0] != 0;
			} else if(uint8Array[0] == 7) {
				return NaN;
			} else if(uint8Array[0] == 8) {
				let fakeDecoded = this.util.utf8decode(fakeUint8Array);
				return parseFloat(this.util.utf8decode(returnUint8Array));
			} else if(uint8Array[0] == 9) {
				return Infinity;
			} else if(uint8Array[0] == 254) {
				let fakeDecoded = this.util.utf8decode(fakeUint8Array);
				return this.util.utf8decode(returnUint8Array);
			} else {
				return returnUint8Array;
			}
		} else {
			let returnArray = [];
			for(let i = 0; i < uint8Array.length; i++) {
				returnArray[i] = uint8Array[i];
			}
			return returnArray;
		}
	}
	
	this.util.pack = (mixed, length) => {
		return this.util.mixedToUint8Array(mixed, true, length);
	}
	
	this.util.unpack = (packed) => {
		return this.util.uint8ArrayToMixed(packed, true);
	}
	
	this.util.objectEquals = (object1, object2) => {
		let deepCompare = () => {
			var i, l, leftChain, rightChain;
			let compare2Objects = (x, y) => {
				var p;
				// remember that NaN === NaN returns false
				// and isNaN(undefined) returns true
				if(isNaN(x) && isNaN(y) && typeof x === 'number' && typeof y === 'number') {
					return true;
				}
				
				// Compare primitives and functions.
				// Check if both arguments link to the same object.
				// Especially useful on the step where we compare prototypes
				if(x === y) {
					return true;
				}
				
				// Works in case when functions are created in constructor.
				// Comparing dates is a common scenario. Another built-ins?
				// We can even handle functions passed across iframes
				if((typeof x === 'function' && typeof y === 'function') ||
				   (x instanceof Date && y instanceof Date) ||
				   (x instanceof RegExp && y instanceof RegExp) ||
				   (x instanceof String && y instanceof String) ||
				   (x instanceof Number && y instanceof Number)) {
					return x.toString() === y.toString();
				}
				
				// At last checking prototypes as good as we can
				if(!(x instanceof Object && y instanceof Object)) {
					return false;
				}
				
				if(x.isPrototypeOf(y) || y.isPrototypeOf(x)) {
					return false;
				}
				
				if(x.constructor !== y.constructor) {
					return false;
				}
				
				if(x.prototype !== y.prototype) {
					return false;
				}
				
				// Check for infinitive linking loops
				if(leftChain.indexOf(x) > -1 || rightChain.indexOf(y) > -1) {
					return false;
				}
				
				// Quick checking of one object being a subset of another.
				// todo: cache the structure of arguments[0] for performance
				for(p in y) {
					if(y.hasOwnProperty(p) !== x.hasOwnProperty(p)) {
						return false;
					} else if(typeof y[p] !== typeof x[p]) {
						return false;
					}
				}
				
				for(p in x) {
					if(y.hasOwnProperty(p) !== x.hasOwnProperty(p)) {
						return false;
					} else if(typeof y[p] !== typeof x[p]) {
						return false;
					}
					
					switch(typeof (x[p])) {
						case 'object':
						case 'function':
							leftChain.push(x);
							rightChain.push(y);
							if(!compare2Objects (x[p], y[p])) {
								return false;
							}
							leftChain.pop();
							rightChain.pop();
							break;
						default:
							if(x[p] !== y[p]) {
								return false;
							}
							break;
					}
				}
				return true;
			}
			if(arguments.length < 1) {
				throw "need two or more arguments to compare";
			}
			for(i = 1, l = arguments.length; i < l; i++) {
				leftChain = []; //Todo: this can be cached
				rightChain = [];
				if(!compare2Objects(arguments[0], arguments[i])) {
					return false;
				}
			}
			return true;
		}
		
		return deepCompare(object1, object2);
	}
	
	this.util.fixArmor = (armored) => {
		armored = armored.replace("Version: OpenPGP.js v4.10.10", "Version: tEnvoy " + this.version).replace("Comment: https://openpgpjs.org", "Comment: https://togatech.org/ (TogaTech tEnvoy)");
		while(armored[0] != "-") {
			armored = armored.substring(1);
		}
		return armored;
	}
	
	this.hash = (mixed, algorithm = "sha256") => {
		if(algorithm == null) {
			algorithm = "sha256";
		}
		if(typeof this.hash[algorithm] == "function") {
			return this.hash[algorithm](mixed);
		} else {
			throw "tEnvoy Fatal Error: argument mixed of method hash is invalid.";
		}
	}
	
	this.hash.sha256 = (mixed) => {
		return new Promise(async (resolve, reject) => {
			if(mixed == null) {
				reject("tEnvoy Fatal Error: argument mixed of method hash.sha256 is required and does not have a default value.");
			}
			resolve(
				this.util.stringToHex(
					this.util.bytesToString(
						await _openpgp.crypto.hash.sha256(
							this.util.mixedToUint8Array(mixed, false)
						).catch((err) => {
							reject(err);
						})
					)
				)
			);
		});
	}
	
	this.hash.sha1 = (mixed) => {
		return new Promise(async (resolve, reject) => {
			if(mixed == null) {
				reject("tEnvoy Fatal Error: argument mixed of method hash.sha1 is required and does not have a default value.");
			}
			resolve(
				this.util.stringToHex(
					this.util.bytesToString(
						await _openpgp.crypto.hash.sha1(
							this.util.mixedToUint8Array(mixed, false)
						).catch((err) => {
							reject(err);
						})
					)
				)
			);
		});
	}
	
	this.hash.sha224 = (mixed) => {
		return new Promise(async (resolve, reject) => {
			if(mixed == null) {
				reject("tEnvoy Fatal Error: argument mixed of method hash.sha224 is required and does not have a default value.");
			}
			resolve(
				this.util.stringToHex(
					this.util.bytesToString(
						await _openpgp.crypto.hash.sha224(
							this.util.mixedToUint8Array(mixed, false)
						).catch((err) => {
							reject(err);
						})
					)
				)
			);
		});
	}
	
	this.hash.sha384 = (mixed) => {
		return new Promise(async (resolve, reject) => {
			if(mixed == null) {
				reject("tEnvoy Fatal Error: argument mixed of method hash.sha384 is required and does not have a default value.");
			}
			resolve(
				this.util.stringToHex(
					this.util.bytesToString(
						await _openpgp.crypto.hash.sha384(
							this.util.mixedToUint8Array(mixed, false)
						).catch((err) => {
							reject(err);
						})
					)
				)
			);
		});
	}
	
	this.hash.sha512 = (mixed) => {
		return new Promise(async (resolve, reject) => {
			if(mixed == null) {
				reject("tEnvoy Fatal Error: argument mixed of method hash.sha512 is required and does not have a default value.");
			}
			resolve(
				this.util.stringToHex(
					this.util.bytesToString(
						await _openpgp.crypto.hash.sha512(
							this.util.mixedToUint8Array(mixed, false)
						).catch((err) => {
							reject(err);
						})
					)
				)
			);
		});
	}
	
	this.hash.md5 = (mixed) => {
		return new Promise(async (resolve, reject) => {
			if(mixed == null) {
				reject("tEnvoy Fatal Error: argument mixed of method hash.md5 is required and does not have a default value.");
			}
			resolve(
				this.util.stringToHex(
					this.util.bytesToString(
						await _openpgp.crypto.hash.md5(
							this.util.mixedToUint8Array(mixed, false)
						).catch((err) => {
							reject(err);
						})
					)
				)
			);
		});
	}
	
	this.hash.ripemd160 = (mixed) => {
		return new Promise(async (resolve, reject) => {
			if(mixed == null) {
				reject("tEnvoy Fatal Error: argument mixed of method hash.ripemd160 is required and does not have a default value.");
			}
			resolve(
				this.util.stringToHex(
					this.util.bytesToString(
						await _openpgp.crypto.hash.ripemd(
							this.util.mixedToUint8Array(mixed, false)
						).catch((err) => {
							reject(err);
						})
					)
				)
			);
		});
	}
	
	this.random = {};
	
	this.random.bytes = (length = 1) => {
		return new Promise(async (resolve, reject) => {
			if(length == null) {
				length = 1;
			}
			if(isNaN(parseInt(length))) {
				length = 0;
			} else {
				length = parseInt(length);
			}
			resolve(await _openpgp.crypto.random.getRandomBytes(length).catch((err) => {
				reject(err);
			}));
		});
	}
	
	this.random.number = (min = 0, max = 1) => {
		return new Promise(async (resolve, reject) => {
			if(min == null) {
				min = 0;
			}
			if(isNaN(parseFloat(min))) {
				min = 0;
			} else {
				min = parseFloat(min);
			}
			if(max == null) {
				max = 0;
			}
			if(isNaN(parseFloat(max))) {
				max = 0;
			} else {
				max = parseFloat(max);
			}
			let random = await this.random.bytes(1).catch((err) => {
				reject(err);
			});
			resolve((random[0] / 255) * (max - min) + min);
		});
	}
	
	this.random.string = (length = 10) => {
		return new Promise(async (resolve, reject) => {
			if(length == null) {
				length = 10;
			}
			if(isNaN(parseInt(length))) {
				length = 10;
			} else {
				length = parseInt(length);
			}
			let result = "";
			let characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
			for(let i = 0; i < length; i++)
			{
				result += characters.charAt(Math.floor(await this.random.number(0, characters.length - 1).catch((err) => {
					reject(err);
				})));
			}
			resolve(result);
		});
	}
	
	this.random.words = (length = 12) => {
		return new Promise(async (resolve, reject) => {
			if(length == null) {
				length = 12;
			}
			if(isNaN(parseInt(length))) {
				length = 12;
			} else {
				length = parseInt(length);
			}
			let words = "";
			for(let i = 0; i < length; i++) {
				words += this.wordsList[Math.floor(await this.random.number(0, 2047).catch((err) => {
					reject(err);
				}))];
				if(i + 1 != length) {
					words += " ";
				}
			}
			resolve(words);
		});
	}
	
	this.keyFactory = {};
	
	this.keyFactory.pbkdf2 = (password, salt, rounds = 150000, size = 32) => {
		if(password == null) {
			throw "tEnvoy Fatal Error: argument password of method keyFactory.pbkdf2 is required and does not have a default value.";
		}
		if(salt == null) {
			throw "tEnvoy Fatal Error: argument salt of method keyFactory.pbkdf2 is required and does not have a default value.";
		}
		if(rounds == null) {
			rounds = 150000;
		}
		if(isNaN(parseInt(rounds))) {
			rounds = 150000;
		} else {
			rounds = parseInt(rounds);
		}
		if(size == null) {
			size = 32;
		}
		if(isNaN(parseInt(size))) {
			size = 32;
		} else {
			size = parseInt(size);
		}
		password = this.util.mixedToUint8Array(password, false);
		salt = this.util.mixedToUint8Array(salt, false);
		return _sha256.pbkdf2(password, salt, rounds, size);
	}
	this.keyFactory.genSeedFromCredentials = (username, password, rounds = 150000, size = 32) => {
		if(username == null) {
			reject("tEnvoy Fatal Error: argument username of method keyFactory.genSeedFromCredentials is required and does not have a default value.");
		}
		if(password == null) {
			reject("tEnvoy Fatal Error: argument password of method keyFactory.genSeedFromCredentials is required and does not have a default value.");
		}
		if(size == null) {
			size = 32;
		}
		if(isNaN(parseInt(size))) {
			size = 32;
		} else {
			size = parseInt(size);
		}
		return this.keyFactory.pbkdf2(password, username, rounds, size);
	}
	
	this.keyFactory.genPGPKeys = (args) => {
		return new Promise(async (resolve, reject) => {
			if(args == null) {
				args = {};
			}
			if(args.passwordProtected == null) {
				args.passwordProtected = [];
			}
			let privateKey;
			let publicKey;
			let privateArmored;
			let publicArmored;
			if(args.keyArmored != null) {
				let key = new tEnvoyPGPKey(args.keyArmored, null, null, [], this);
				let type = key.getType();
				if(type == "private") {
					privateArmored = await key.getPrivateArmored(args.password);
					publicArmored = await key.getPublicArmored(args.password);
				} else if(type == "public") {
					publicArmored = await key.getPublicArmored(args.password);
				} else {
					reject("tEnvoy Fatal Error: argument key of object args of method keyFactory.genPGPKeys must either be public or private. For aes keys, use keyFactory.genPGPSymmetricKey instead.");
				}
				key.destroy();
			} else {
				if(args.options == null) {
					args.options = {
						curve: "curve25519"
					}
				}
				if(args.users == null && args.options.userIds == null) {
					args.users = [{}];
				}
				if(args.users == null && args.options.userIds != null) {
					args.users = args.options.userIds;
				}
				if(args.users != null && args.options.userIds != null) {
					args.options.userIds = args.options.userIds.filter(id => args.users.find(i => i.name == id.name && i.email == id.email && i.comment == id.comment) == null);
					args.users = args.users.concat(args.options.userIds);
				}
				for(let i = 0; i < args.users.length; i++) {
					let name = args.users[i].name || "";
					let email = args.users[i].email || "";
					let comment = args.users[i].comment || "";
					args.users[i] = {name: name, email: email, comment: comment};
				}
				args.options.userIds = args.users;
				
				let openpgpkey = await _openpgp.generateKey(args.options).catch((err) => {
					reject(err);
				});
				privateArmored = this.util.fixArmor(openpgpkey.privateKeyArmored)
				publicArmored = this.util.fixArmor(openpgpkey.publicKeyArmored);
			}
			if(args.password == null) {
				if(privateArmored != null) {
					privateKey = new tEnvoyPGPKey(privateArmored, "private", args.password, args.passwordProtected, this);
				}
				publicKey = new tEnvoyPGPKey(publicArmored, "public", args.password, args.passwordProtected, this);
			} else {
				if(privateArmored != null) {
					let encryptedPrivateKey = await _openpgp.encrypt({
						message: await _openpgp.message.fromText(privateArmored),
						passwords: [args.password]
					}).catch((err) => {
						reject(err);
					});
					privateKey = new tEnvoyPGPKey(this.util.fixArmor(encryptedPrivateKey.data), "private", args.password, args.passwordProtected, this);
				}
				let encryptedPublicKey = await _openpgp.encrypt({
					message: await _openpgp.message.fromText(publicArmored),
					passwords: [args.password]
				}).catch((err) => {
					reject(err);
				});
				publicKey = new tEnvoyPGPKey(this.util.fixArmor(encryptedPublicKey.data), "public", args.password, args.passwordProtected, this);
			}
			resolve({
				privateKey: privateKey,
				publicKey: publicKey
			});
		});
	}
	
	this.keyFactory.genPGPSymmetricKey = (args) => {
		return new Promise(async (resolve, reject) => {
			if(args == null) {
				args = {};
			}
			if(args.passwordProtected == null) {
				args.passwordProtected = [];
			}
			if(args.key == null) {
				reject("tEnvoy Fatal Error: argument key of object args of method keyFactory.genPGPSymmetricKey is required and does not have a default value.");
			}
			if(args.password == null) {
				resolve(new tEnvoyPGPKey(args.key, "aes", null, args.passwordProtected, this));
			} else {
				let encryptedKey = await _openpgp.encrypt({
					message: await _openpgp.message.fromText(args.key),
					passwords: [args.password]
				}).catch((err) => {
					reject(err);
				});
				resolve(new tEnvoyPGPKey(this.util.fixArmor(encryptedKey.data), "aes", args.password, args.passwordProtected, this));
			}
		});
	}
	
	this.keyFactory.genNaClKeys = (args) => {
		if(args == null) {
			args = {};
		}
		if(args.passwordProtected == null) {
			args.passwordProtected = [];
		}
		let privateKey;
		let publicKey;
		let privateSigningKey;
		let publicSigningKey;
		let naclKeyPair;
		if(args.key != null) {
			if(args.keyType != null) {
				if(args.keyType == "private") {
					privateKey = new tEnvoyNaClKey(args.key, "private", args.password, args.passwordProtected, this);
					publicKey = privateKey.toPublic();
				} else if(args.keyType == "public") {
					publicKey = new tEnvoyNaClKey(args.key, "public", args.password, args.passwordProtected, this);
				} else if(args.keyType == "privateSigning") {
					privateSigningKey = new tEnvoyNaClSigningKey(args.key, "private", args.password, args.passwordProtected, this);
				} else if(args.keyType == "publicSigning") {
					publicSigningKey = new tEnvoyNaClSigningKey(args.key, "public", args.password, args.passwordProtected, this);
				} else {
					throw "tEnvoy Fatal Error: argument keyType of object args of method keyFactory.genNaClKeys must either be private, public, privateSigning, or publicSigning. For secret (or shared) keys, use keyFactory.genNaClSymmetricKey instead.";
				}
			} else {
				throw "tEnvoy Fatal Error: argument keyType of object args of method keyFactory.genNaClKeys is required when using args.key and does not have a default value.";
			}
		} else {
			if(args.seed == null) {
				naclKeyPair = _nacl.box.keyPair();
			} else {
				naclKeyPair = _nacl.box.keyPair.fromSecretKey(args.seed);
			}
			privateKey = new tEnvoyNaClKey(naclKeyPair.secretKey, "private", args.password, args.passwordProtected, this);
			publicKey = new tEnvoyNaClKey(naclKeyPair.publicKey, "public", args.password, args.passwordProtected, this);
		}
		if(privateKey != null) {
			let signingKeys = privateKey.genSigningKeys(args.password);
			privateSigningKey = signingKeys.privateKey;
			publicSigningKey = signingKeys.publicKey;
		}
		return {
			privateKey: privateKey,
			publicKey: publicKey,
			privateSigningKey: privateSigningKey,
			publicSigningKey: publicSigningKey
		};
	}
	
	this.keyFactory.genNaClSymmetricKey = (args) => {
		if(args == null) {
			args = {};
		}
		if(args.passwordProtected == null) {
			args.passwordProtected = [];
		}
		if(args.key == null) {
			throw "tEnvoy Fatal Error: argument key of object args of method keyFactory.genNaClSymmetricKey is required and does not have a default value.";
		}
		return new tEnvoyNaClKey(args.key, "secret", args.password, args.passwordProtected, this);
	}
}

function tEnvoyPGPKey(keyArmored, type = "aes", password = null, passwordProtected = [], tEnvoy = TogaTech.tEnvoy) {
	let _keyArmored;
	let _password;
	let _passwordProtected;
	let _type;
	let _assertPassword;
	let _getKey;
	let _setKey;
	let _getPassword;
	let _setPassword;
	let _tEnvoy = tEnvoy;
	let _openpgp = _tEnvoy.core.openpgp;

	this.destroy = (password = null) => {
		let assertion = _assertPassword("destroy", password);
		if(assertion.proceed) {
			if(_keyArmored != null) {
				for(let i = 0; i < _keyArmored.length; i++) {
					_keyArmored[i] = 0;
				}
			}
			if(_password != null) {
				for(let i = 0; i < _password.length; i++) {
					_password[i] = 0;
				}
			}
			for(method in this) {
				delete this[method];
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.toString = () => {
		return `[tEnvoyPGPKey ${_type}]`;
	}
	
	this.getType = () => {
		return _type;
	}
	
	this.getPasswordProtected = () => {
		return _tEnvoy.util.arrayDeepCopy(_passwordProtected);
	}

	this.setPasswordProtected = (passwordProtected, password = null) => {
		let assertion = _assertPassword("setPasswordProtected", password);
		if(assertion.proceed) {
			_passwordProtected = [];
			let protectable = [];
			if(_type == "private") {
				protectable = ["destroy", "getId", "getPublic", "encrypt", "decrypt", "sign", "verify"];
			} else if(_type == "public") {
				protectable = ["destroy", "getId", "encrypt", "verify"];
			} else if(_type == "aes") {
				protectable = ["destroy", "encrypt", "decrypt"];
			}
			if(passwordProtected == null) {
				passwordProtected = [];
			}
			for(let i = 0; i < passwordProtected.length; i++) {
				if(protectable.includes(passwordProtected[i])) {
					_passwordProtected.push(passwordProtected[i]);
				}
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.getId = (password = null) => {
		return new Promise(async (resolve, reject) => {
			if(_type == "private" || _type == "public") {
				let assertion = _assertPassword("getId", password);
				if(assertion.proceed) {
					let publicKey = await this.getPublic(_getPassword());
					resolve(publicKey.getKeyId().toHex());
				} else {
					reject(assertion.error);
				}
			} else {
				reject("tEnvoyPGPKey Fatal Error: Key does not have an asymmetric component.");
			}
		});
	}
	
	this.getKey = (password = null) => {
		return new Promise(async (resolve, reject) => {
			if(_type == "aes") {
				if(_getPassword() == null) {
					resolve(_getKey());
				} else {
					let assertion = _assertPassword("getKey", password);
					if(assertion.proceed) {
						let decryptedKey = await _openpgp.decrypt({
							message: await _openpgp.message.readArmored(_getKey()),
							passwords: [_getPassword()]
						}).catch((err) => {
							reject(err);
						});
						resolve(decryptedKey.data);
					} else {
						reject(assertion.error);
					}
				}
			} else {
				reject("tEnvoyPGPKey Fatal Error: Key does not have a symmetric component.");
			}
		});
	}
	
	this.getPrivate = (password = null) => {
		return new Promise(async (resolve, reject) => {
			let privateKeyArmored = await this.getPrivateArmored(password).catch((err) => {
				reject(err);
			});
			let openpgpkey = await _openpgp.key.readArmored(privateKeyArmored).catch((err) => {
				reject(err);
			});
			resolve(openpgpkey.keys[0]);
		});
	}
	
	this.setPrivate = (privateKey, password = null) => {
		return new Promise(async (resolve, reject) => {
			await this.setPrivateArmored(privateKey.armor(), password).catch((err) => {
				reject(err);
			});
			resolve();
		});
	}
	
	this.getPrivateArmored = (password = null) => {
		return new Promise(async (resolve, reject) => {
			if(_type == "private") {
				if(_getPassword() == null) {
					resolve(_getKey());
				} else {
					let assertion = _assertPassword("getPrivate", password);
					if(assertion.proceed) {
						let decryptedKey = await _openpgp.decrypt({
							message: await _openpgp.message.readArmored(keyArmored),
							passwords: [_getPassword()]
						}).catch((err) => {
							reject(err);
						});
						resolve(decryptedKey.data);
					} else {
						reject(assertion.error);
					}
				}
			} else {
				reject("tEnvoyPGPKey Fatal Error: Key does not have a private component.");
			}
		});
	}
	
	this.setPrivateArmored = (keyArmored, password = null) => {
		return new Promise(async (resolve, reject) => {
			if(_type == "private") {
				if(keyArmored == null) {
					reject("tEnvoyPGPKey Fatal Error: argument keyArmored of method setPrivateArmored is required and does not have a default value.");
				}
				keyArmored = _tEnvoy.util.fixArmor(keyArmored);
				if(_getPassword() == null) {
					_setKey(keyArmored);
					resolve();
				} else {
					let assertion = _assertPassword("setPrivate", password);
					if(assertion.proceed) {
						let privateKeyEncrypted = await _openpgp.encrypt({
							message: await _openpgp.message.fromText(keyArmored),
							passwords: [_getPassword()]
						}).catch((err) => {
							reject(err);
						});
						_setKey(_tEnvoy.util.fixArmor(privateKeyEncrypted.data));
						resolve();
					} else {
						reject(assertion.error);
					}
				}
			} else {
				reject("tEnvoyPGPKey Fatal Error: Key does not have a private component.");
			}
		});
	}
	
	this.getPublic = (password = null) => {
		return new Promise(async (resolve, reject) => {
			let assertion = _assertPassword("getPublic", password);
			if(assertion.proceed) {
				if(_type == "private") {
					let key = await this.getPrivate(_getPassword());
					resolve(key.toPublic());
				} else if(_type == "public") {
					let publicKeyArmored;
					if(_getPassword() == null) {
						publicKeyArmored = _getKey();
					} else {
						let decryptedKey = await _openpgp.decrypt({
							message: await _openpgp.message.readArmored(_getKey()),
							passwords: [_getPassword()]
						}).catch((err) => {
							reject(err);
						});
						publicKeyArmored = decryptedKey.data;
					}
					let openpgpkey = await _openpgp.key.readArmored(publicKeyArmored).catch((err) => {
						reject(err);
					});
					resolve(openpgpkey.keys[0]);
				} else {
					reject("tEnvoyPGPKey Fatal Error: Key does not have a public component.");
				}
			} else {
				reject(assertion.error);
			}
		});
	}
	
	this.setPublic = (publicKey, password = null) => {
		return new Promise(async (resolve, reject) => {
			let assertion = _assertPassword("setPublic", password);
			if(assertion.proceed) {
				if(_type == "public") {
					if(_getPassword() == null) {
						_setKey(_tEnvoy.util.fixArmor(publicKey.armor()));
						resolve();
					} else {
						let publicKeyEncrypted = await _openpgp.encrypt({
							message: await _openpgp.message.fromText(publicKey.armor()),
							passwords: [_getPassword()]
						}).catch((err) => {
							reject(err);
						});
						_setKey(_tEnvoy.util.fixArmor(publicKeyEncrypted.data));
						resolve();
					}
				} else if(_type == "private") {
					reject("tEnvoyPGPKey Fatal Error: Key has a public component that depends on the private component.");
				} else {
					reject("tEnvoyPGPKey Fatal Error: Key does not have a public component.");
				}
			} else {
				reject(assertion.error);
			}
		});
	}
	
	this.getPublicArmored = (password = null) => {
		return new Promise(async (resolve, reject) => {
			let key = await this.getPublic(password).catch((err) => {
				reject(err);
			});
			resolve(_tEnvoy.util.fixArmor(key.armor()));
		});
	}
	
	this.setPublicArmored = (keyArmored, password = null) => {
		return new Promise(async (resolve, reject) => {
			if(keyArmored == null) {
				reject("tEnvoyPGPKey Fatal Error: argument keyArmored of method setPublicArmored is required and does not have a default value.");
			} else {
				keyArmored = _tEnvoy.util.fixArmor(keyArmored);
				let openpgpkey = await _openpgp.key.readArmored(keyArmored).catch((err) => {
					reject(err);
				});
				await this.setPublic(openpgpkey.keys[0], password);
				resolve();
			}
		});
	}
	
	this.encrypt = (message, password = null) => {
		return new Promise(async (resolve, reject) => {
			let assertion = _assertPassword("encrypt", password);
			if(assertion.proceed) {
				let encryptKey;
				let encrypted;
				if(_type == "aes") {
					encryptKey = await this.getKey(_getPassword()).catch((err) => {
						reject(err);
					});
					encrypted = await _openpgp.encrypt({
						message: await _openpgp.message.fromText(message),
						passwords: [encryptKey]
					}).catch((err) => {
						reject(err);
					});
				} else {
					encryptKey = await this.getPublic(_getPassword()).catch((err) => {
						reject(err);
					});
					encrypted = await _openpgp.encrypt({
						message: _openpgp.message.fromText(message),
						publicKeys: encryptKey
					}).catch((err) => {
						reject(err);
					});
				}
				resolve(_tEnvoy.util.fixArmor(encrypted.data));
			}
		});
	}
	
	this.decrypt = (message, password = null) => {
		return new Promise(async (resolve, reject) => {
			let assertion = _assertPassword("decrypt", password);
			if(assertion.proceed) {
				message = _tEnvoy.util.fixArmor(message);
				let decryptKey;
				let decrypted;
				if(_type == "aes") {
					decryptKey = await this.getKey(_getPassword()).catch((err) => {
						reject(err);
					});
					decrypted = await _openpgp.decrypt({
						message: await _openpgp.message.readArmored(message),
						passwords: [decryptKey]
					}).catch((err) => {
						reject(err);
					});
				} else {
					decryptKey = await this.getPrivate(_getPassword()).catch((err) => {
						reject(err);
					});
					decrypted = await _openpgp.decrypt({
						message: await _openpgp.message.readArmored(message),
						privateKeys: decryptKey
					}).catch((err) => {
						reject(err);
					});
				}
				resolve(decrypted.data);
			} else {
				reject(assertion.error);
			}
		});
	}
	
	this.sign = (message, password = null) => {
		return new Promise(async (resolve, reject) => {
			let assertion = _assertPassword("sign", password);
			if(assertion.proceed) {
				let signKey;
				if(_type == "aes") {
					reject("tEnvoyPGPKey Fatal Error: Key does not have an asymmetric component.");
				} else {
					signKey = await this.getPrivate(_getPassword());
					let signed = await _openpgp.sign({
						message: await _openpgp.cleartext.fromText(message),
						privateKeys: signKey
					});
					resolve(_tEnvoy.util.fixArmor(signed.data));
				}
			} else {
				reject(assertion.error);
			}
		});
	}
	
	this.verify = (message, password = null) => {
		return new Promise(async (resolve, reject) => {
			let assertion = _assertPassword("verify", password);
			if(assertion.proceed) {
				message = _tEnvoy.util.fixArmor(message);
				let verifyKey;
				if(_type == "aes") {
					reject("tEnvoyPGPKey Fatal Error: Key does not have an asymmetric component.");
				} else {
					verifyKey = await this.getPublic(_getPassword());
					let verified = await _openpgp.verify({
						message: await _openpgp.cleartext.readArmored(message),
						publicKeys: verifyKey
					});
					if(verified.signatures[0] && verified.signatures[0].valid) {
						resolve({
							verified: true,
							keyid: verified.signatures[0].keyid.toHex(),
							signatures: verified.signatures,
							content: message.split("\n")[3]
						});
					} else {
						resolve({
							verified: false
						});
					}
				}
			} else {
				reject(assertion.error);
			}
		});
	}
	
	this.toPublic = (password = null) => {
		return new Promise(async (resolve, reject) => {
			resolve(new tEnvoyPGPKey(await this.getPublicArmored(password), "public", _getPassword(), _passwordProtected, _tEnvoy));
		});
	}
	
	if(keyArmored.indexOf("-----BEGIN PGP PRIVATE KEY BLOCK-----") == 0) {
		_type = "private";
		keyArmored = _tEnvoy.util.fixArmor(keyArmored);
	} else if(keyArmored.indexOf("-----BEGIN PGP PUBLIC KEY BLOCK-----") == 0) {
		_type = "public";
		keyArmored = _tEnvoy.util.fixArmor(keyArmored);
	} else if(keyArmored.indexOf("-----BEGIN PGP MESSAGE-----") == 0) {
		_type = type || "aes";
		keyArmored = _tEnvoy.util.fixArmor(keyArmored);
	} else {
		_type = "aes";
	}
	if(!["public", "private", "aes"].includes(_type)) {
		throw "tEnvoyPGPKey Fatal Error: argument type of method constructor is invalid.";
	} else {
		_getKey = () => {
			return _tEnvoy.util.unpack(_keyArmored);
		}

		_setKey = (newKey) => {
			_keyArmored = _tEnvoy.util.pack(newKey);
		}

		_getPassword = () => {
			return _tEnvoy.util.unpack(_password);
		}

		_setPassword = (newPassword) => {
			_password = _tEnvoy.util.pack(newPassword);
		}

		_setPassword(password);
		_setKey(keyArmored);
		_passwordProtected = [];
		let protectable = [];
		if(_type == "private") {
			protectable = ["destroy", "getId", "getPublic", "encrypt", "decrypt", "sign", "verify"];
		} else if(_type == "public") {
			protectable = ["destroy", "getId", "encrypt", "verify"];
		} else if(_type == "aes") {
			protectable = ["destroy", "encrypt", "decrypt"];
		}
		if(passwordProtected == null) {
			passwordProtected = [];
		}
		for(let i = 0; i < passwordProtected.length; i++) {
			if(protectable.includes(passwordProtected[i])) {
				_passwordProtected.push(passwordProtected[i]);
			}
		}
		_assertPassword = (methodName, password) => {
			if(_getPassword() == null) {
				return {
					proceed: true
				};
			} else {
				let alwaysProtected;
				if(_type == "private") {
					alwaysProtected = ["getPrivate", "setPrivate", "setPasswordProtected"];
				} else if(_type == "public") {
					alwaysProtected = ["getPublic", "setPublic", "setPasswordProtected"];
				} else if(_type == "aes") {
					alwaysProtected = ["getKey"];
				}
				if(alwaysProtected.includes(methodName) || _passwordProtected.includes(methodName)) {
					if(password == null) {
						return {
							proceed: false,
							error: "tEnvoyPGPKey Fatal Error: Key is password-protected for method " + methodName + ", and no password was specified."
						};
					} else if(!_tEnvoy.util.compareConstant(password, _getPassword())) {
						return {
							proceed: false,
							error: "tEnvoyPGPKey Fatal Error: Key is password-protected for method " + methodName + ", and an incorrect password was specified."
						};
					} else {
						return {
							proceed: true
						};
					}
				} else {
					return {
						proceed: true
					};
				}
			}
		}

		if(_type != "aes") {
			this.getPublic(_getPassword());
		}
	}
}

function tEnvoyNaClKey(key, type = "secret", password = null, passwordProtected = [], tEnvoy = TogaTech.tEnvoy) {
	let _key;
	let _nonce;
	let _password;
	let _passwordProtected;
	let _type;
	let _assertPassword;
	let _getKey;
	let _setKey;
	let _getPassword;
	let _setPassword;
	let _tEnvoy = tEnvoy;
	let _nacl = _tEnvoy.core.nacl;

	this.destroy = (password = null) => {
		let assertion = _assertPassword("destroy", password);
		if(assertion.proceed) {
			if(_key != null) {
				for(let i = 0; i < _key.length; i++) {
					_key[i] = 0;
				}
			}
			if(_password != null) {
				for(let i = 0; i < _password.length; i++) {
					_password[i] = 0;
				}
			}
			for(method in this) {
				delete this[method];
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.toString = () => {
		return `[tEnvoyNaClKey ${_type}]`;
	}
	
	this.getType = () => {
		return _type;
	}
	
	this.getPasswordProtected = () => {
		return _tEnvoy.util.arrayDeepCopy(_passwordProtected);
	}

	this.setPasswordProtected = (passwordProtected, password = null) => {
		let assertion = _assertPassword("setPasswordProtected", password);
		if(assertion.proceed) {
			_passwordProtected = [];
			let protectable = [];
			if(_type == "private" || _type == "shared" || _type == "secret") {
				protectable = ["destroy", "getPublic", "encrypt", "decrypt", "encryptEphemeral", "decryptEphemeral", "genSigningKey", "genSharedKey", "sign", "verify"];
			} else if(_type == "public") {
				protectable = ["destroy", "encrypt", "genSharedKey", "verify"];
			}
			if(passwordProtected == null) {
				passwordProtected = [];
			}
			for(let i = 0; i < passwordProtected.length; i++) {
				if(protectable.includes(passwordProtected[i])) {
					_passwordProtected.push(passwordProtected[i]);
				}
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.getPrivate = (password = null) => {
		let assertion = _assertPassword("getPrivate", password);
		if(assertion.proceed) {
			if(_type == "private" || _type == "secret" || _type == "shared") {
				if(_getPassword() == null) {
					if(_getKey() instanceof Array || _getKey() instanceof Uint8Array) {
						return _tEnvoy.util.arrayDeepCopy(_getKey());
					} else {
						return _getKey();
					}
				} else {
					let decryptionKey = new tEnvoyNaClKey(_getPassword(), "secret", null, [], _tEnvoy);
					let decrypted = decryptionKey.decrypt(_getKey());
					decryptionKey.destroy();
					if(_tEnvoy.util.bytesToHex(decrypted.nonce) == _tEnvoy.util.bytesToHex(_nonce)) {
						return decrypted.message;
					} else {
						throw "tEnvoyNaClKey Fatal Error: The encrypted key was tampered with, and the nonce is invalid.";
					}
				}
			} else {
				throw "tEnvoyNaClKey Fatal Error: Key does not have a private, secret, or shared component.";
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.setPrivate = (privateKey, password = null) => {
		let assertion = _assertPassword("setPrivate", password);
		if(assertion.proceed) {
			if(privateKey == null) {
				throw "tEnvoyNaClKey Fatal Error: argument privateKey of method setPrivate is required and does not have a default value.";
			}
			privateKey = _tEnvoy.util.mixedToUint8Array(privateKey, false);
			if(privateKey.length != 32) {
				throw "tEnvoyNaClKey Fatal Error: argument privateKey of method setPrivate is invalid, length should be 32 (was " + privateKey.length + ").";
			}
			if(_type == "private" || _type == "secret" || _type == "shared") {
				if(_getPassword() == null) {
					_setKey(privateKey);
				} else {
					_nonce = _nacl.randomBytes(12);
					let encryptionKey = new tEnvoyNaClKey(_getPassword(), "secret", null, [], _tEnvoy);
					_setKey(encryptionKey.encrypt(privateKey, _nonce));
					encryptionKey.destroy();
				}
			} else {
				throw "tEnvoyNaClKey Fatal Error: Key does not have a private, secret, or shared component.";
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.getPublic = (password = null) => {
		let assertion = _assertPassword("getPublic", password);
		if(assertion.proceed) {
			if(_type == "private") {
				return _nacl.box.keyPair.fromSecretKey(this.getPrivate(_getPassword())).publicKey;
			} else if(_type == "public") {
				if(_getPassword() == null) {
					return _tEnvoy.util.arrayDeepCopy(_getKey());
				} else {
					let decryptionKey = new tEnvoyNaClKey(_getPassword(), "secret", null, [], _tEnvoy);
					let decrypted = decryptionKey.decrypt(_getKey());
					decryptionKey.destroy();
					if(_tEnvoy.util.bytesToHex(decrypted.nonce) == _tEnvoy.util.bytesToHex(_nonce)) {
						return decrypted.message;
					} else {
						throw "tEnvoyNaClKey Fatal Error: The encrypted key was tampered with, and the nonce is invalid.";
					}
				}
			} else {
				throw "tEnvoyNaClKey Fatal Error: Key does not have a public component.";
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.setPublic = (publicKey, password = null) => {
		let assertion = _assertPassword("setPublic", password);
		if(assertion.proceed) {
			if(publicKey == null) {
				throw "tEnvoyNaClKey Fatal Error: argument publicKey of method setPublic is required and does not have a default value.";
			}
			publicKey = _tEnvoy.util.mixedToUint8Array(publicKey, false);
			if(publicKey.length != 32) {
				throw "tEnvoyNaClKey Fatal Error: argument publicKey of method setPublic is invalid, length should be 32 (was " + publicKey.length + ").";
			}
			if(_type == "private") {
				throw "tEnvoyNaClKey Fatal Error: Key has a public component that depends on the private component.";
			} else if(_type == "public") {
				if(_getPassword() == null) {
					_setKey(publicKey);
				} else {
					_nonce = _nacl.randomBytes(12);
					let encryptionKey = new tEnvoyNaClKey(_getPassword(), "secret", null, [], _tEnvoy);
					_setKey(encryptionKey.encrypt(publicKey, _nonce));
					encryptionKey.destroy();
				}
			} else {
				throw "tEnvoyNaClKey Fatal Error: Key does not have a public component.";
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.encrypt = (message, nonce, password = null) => {
		let assertion = _assertPassword("encrypt", password);
		if(assertion.proceed) {
			if(message == null) {
				throw "tEnvoyNaClKey Fatal Error: argument message of method encrypt is required and does not have a default value.";
			}
			if(nonce == null) {
				throw "tEnvoyNaClKey Fatal Error: argument nonce of method encrypt is required and does not have a default value.";
			}
			let paddingLength = _tEnvoy.util.pack(message).length;
			let randomPadding = _nacl.randomBytes(1)[0] % 16;
			paddingLength = 32 + 32 * parseInt(paddingLength / 32);
			paddingLength += randomPadding;
			message = _tEnvoy.util.pack(message, paddingLength);
			let nonceCheck = _tEnvoy.util.pack(nonce);
			if(_type == "shared") {
				if(nonceCheck.length > _nacl.box.nonceLength) {
					throw "tEnvoyNaClKey Fatal Error: Nonce is too long, ensure that nonce length is under " + _nacl.box.nonceLength + " (was " + nonceCheck.length + ").";
				}
				nonce = _tEnvoy.util.pack(nonce, _nacl.box.nonceLength);
			} else if(_type == "secret") {
				if(nonceCheck.length > _nacl.secretbox.nonceLength) {
					throw "tEnvoyNaClKey Fatal Error: Nonce is too long, ensure that nonce length is under " + _nacl.secretbox.nonceLength + " (was " + nonceCheck.length + ").";
				}
				nonce = _tEnvoy.util.pack(nonce, _nacl.secretbox.nonceLength);
			}
			if(_type == "shared") {
				return _tEnvoy.util.bytesToHex(nonce) + "::" + _tEnvoy.util.bytesToHex(_nacl.box.after(message, nonce, this.getPrivate(_getPassword())));
			} else if(_type == "secret") {
				return _tEnvoy.util.bytesToHex(nonce) + "::" + _tEnvoy.util.bytesToHex(_nacl.secretbox(message, nonce, _tEnvoy.util.pack(this.getPrivate(_getPassword()), 32)));
			} else {
				throw "tEnvoyNaClKey Fatal Error: Key cannot be used for encryption, only secret or shared keys can be used to encrypt.";
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.decrypt = (encrypted, password = null) => {
		let assertion = _assertPassword("decrypt", password);
		if(assertion.proceed) {
			if(encrypted == null) {
				throw "tEnvoyNaClKey Fatal Error: argument encrypted of method decrypt is required and does not have a default value.";
			}
			if(encrypted.split("::").length != 2) {
				throw "tEnvoyNaClKey Fatal Error: Invalid encrypted message.";
			}
			let nonce = _tEnvoy.util.mixedToUint8Array(_tEnvoy.util.hexToBytes(encrypted.split("::")[0]), false);
			let encryptedContent = _tEnvoy.util.hexToBytes(encrypted.split("::")[1]);
			if(_type == "shared") {
				return {
					message: _tEnvoy.util.unpack(_nacl.box.open.after(encryptedContent, nonce, this.getPrivate(_getPassword()))),
					nonce: _tEnvoy.util.unpack(_tEnvoy.util.hexToBytes(encrypted.split("::")[0]))
				};
			} else if(_type == "secret") {
				return {
					message: _tEnvoy.util.unpack(_nacl.secretbox.open(encryptedContent, nonce, _tEnvoy.util.pack(this.getPrivate(_getPassword()), 32))),
					nonce: _tEnvoy.util.unpack(_tEnvoy.util.hexToBytes(encrypted.split("::")[0]))
				};
			} else {
				throw "tEnvoyNaClKey Fatal Error: Key cannot be used for decryption, only secret or shared keys can be used to decrypt.";
			}
		} else {
			throw assertion.error;
		}
	}

	this.encryptEphemeral = (message, nonce, password = null) => {
		let assertion = _assertPassword("encryptEphemeral", password);
		if(assertion.proceed) {
			let ephemeralKeys = _tEnvoy.keyFactory.genNaClKeys({
				password: _getPassword(),
				passwordProtected: _passwordProtected
			});
			if(_type == "public") {
				let sharedKey = this.genSharedKey(ephemeralKeys.privateKey, _getPassword(), _getPassword());
				return sharedKey.encrypt(message, nonce, _getPassword()) + "::" + _tEnvoy.util.bytesToHex(ephemeralKeys.publicKey.getPublic(_getPassword()));
			} else if(_type == "private") {
				let sharedKey = this.toPublic(_getPassword()).genSharedKey(ephemeralKeys.privateKey, _getPassword(), _getPassword());
				return sharedKey.encrypt(message, nonce, _getPassword()) + "::" + _tEnvoy.util.bytesToHex(ephemeralKeys.publicKey.getPublic(_getPassword()));
			} else {
				throw "tEnvoyNaClKey Fatal Error: Key cannot be used for ephemeral encryption, only public or private keys can be used to encrypt ephemerally.";
			}
			let sharedKey = publicKey.genSharedKey(ephemeralKeys.privateKey, _getPassword(), _getPassword());
			return sharedKey.encrypt(message, nonce, _getPassword()) + "::" + _tEnvoy.util.bytesToHex(ephemeralKeys.publicKey.getPublic(_getPassword()));
		} else {
			throw assertion.error;
		}
	}

	this.decryptEphemeral = (encryptedEphemeral, password = null) => {
		let assertion = _assertPassword("decryptEphemeral", password);
		if(assertion.proceed) {
			if(encryptedEphemeral == null) {
				throw "tEnvoyNaClKey Fatal Error: argument encryptedEphemeral of method decryptEphemeral is required and does not have a default value.";
			}
			if(encryptedEphemeral.split("::").length != 3) {
				throw "tEnvoyNaClKey Fatal Error: Invalid ephemeral encrypted message.";
			}
			let encrypted = encryptedEphemeral.split("::").slice(0, 2).join("::");
			let ephemeralKey = new tEnvoyNaClKey(_tEnvoy.util.hexToBytes(encryptedEphemeral.split("::")[2]), "public", _getPassword(), _passwordProtected, _tEnvoy);
			if(_type == "private") {
				let sharedKey = this.genSharedKey(ephemeralKey, _getPassword(), _getPassword());
				ephemeralKey.destroy();
				return sharedKey.decrypt(encrypted);
			} else {
				ephemeralKey.destroy();
				throw "tEnvoyNaClKey Fatal Error: Key cannot be used for ephemeral decryption, only private keys can be used to decrypt ephemerally.";
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.genSigningKeys = (password = null) => {
		let assertion = _assertPassword("genSigningKey", password);
		if(assertion.proceed) {
			if(_type != "secret") {
				let signingKeys = _nacl.sign.keyPair.fromSeed(this.getPrivate(_getPassword()));
				let privateKey = new tEnvoyNaClSigningKey(signingKeys.secretKey, "private", _getPassword(), _passwordProtected, _tEnvoy);
				let publicKey = new tEnvoyNaClSigningKey(signingKeys.publicKey, "public", _getPassword(), _passwordProtected, _tEnvoy);
				return {
					privateKey: privateKey,
					publicKey: publicKey
				};
			} else {
				throw "tEnvoyNaClKey Fatal Error: Secret key cannot be used to generate signing keys.";
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.genSharedKey = (otherKey, otherKeyPassword = null, password = null) => {
		let assertion = _assertPassword("genSharedKey", password);
		if(assertion.proceed) {
			if(otherKey == null) {
				throw "tEnvoyNaClKey Fatal Error: argument otherKey of method genSharedKey is required and does not have a default value.";
			}
			if(otherKey instanceof tEnvoyNaClKey) {
				if(_type == "public" && otherKey.getType() == "private") {
					let sharedKey = _nacl.box.before(this.getPublic(_getPassword()), otherKey.getPrivate(otherKeyPassword));
					return new tEnvoyNaClKey(sharedKey, "shared", _getPassword(), _passwordProtected, _tEnvoy);
				} else if(_type == "private" && otherKey.getType() == "public") {
					let sharedKey = _nacl.box.before(otherKey.getPublic(otherKeyPassword), this.getPrivate(_getPassword()));
					return new tEnvoyNaClKey(sharedKey, "shared", _getPassword(), _passwordProtected, _tEnvoy);
				} else {
					throw "tEnvoyNaClKey Fatal Error: Incompatible key types, one key should be public, and the other should be private.";
				}
			} else {
				throw "tEnvoyNaClKey Fatal Error: Incompatible key types, both keys should be of type tEnvoyNaClKey.";
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.toPublic = (password = null) => {
		return new tEnvoyNaClKey(this.getPublic(password), "public", _getPassword(), _passwordProtected, _tEnvoy);
	}
	
	if(!["public", "private", "secret", "shared"].includes(type)) {
		throw "tEnvoyNaClKey Fatal Error: argument type of method constructor is invalid.";
	} else {
		_getKey = () => {
			return _tEnvoy.util.unpack(_key);
		}

		_setKey = (newKey) => {
			_key = _tEnvoy.util.pack(newKey);
		}

		_getPassword = () => {
			return _tEnvoy.util.unpack(_password);
		}

		_setPassword = (newPassword) => {
			_password = _tEnvoy.util.pack(newPassword);
		}

		_setPassword(password);
		if(_type == "secret") {
			key = _tEnvoy.util.pack(key, 32);
		}
		if(password == null) {
			_setKey(key);
		} else {
			_nonce = _nacl.randomBytes(12);
			let encryptionKey = new tEnvoyNaClKey(password, "secret", null, [], _tEnvoy);
			_setKey(encryptionKey.encrypt(key, _nonce));
			encryptionKey.destroy();
		}
		_type = type;
		_passwordProtected = [];
		let protectable = [];
		if(_type == "private" || _type == "shared" || _type == "secret") {
			protectable = ["destroy", "getPublic", "encrypt", "decrypt", "encryptEphemeral", "decryptEphemeral", "genSigningKey", "genSharedKey", "sign", "verify"];
		} else if(_type == "public") {
			protectable = ["destroy", "encrypt", "genSharedKey", "verify"];
		}
		if(passwordProtected == null) {
			passwordProtected = [];
		}
		for(let i = 0; i < passwordProtected.length; i++) {
			if(protectable.includes(passwordProtected[i])) {
				_passwordProtected.push(passwordProtected[i]);
			}
		}
		_assertPassword = (methodName, password = null) => {
			if(_getPassword() == null) {
				return {
					proceed: true
				};
			} else {
				let alwaysProtected;
				if(_type == "private" || _type == "shared" || _type == "secret") {
					alwaysProtected = ["getPrivate", "setPrivate", "setPasswordProtected"];
				} else if(_type == "public") {
					alwaysProtected = ["getPublic", "setPublic", "setPasswordProtected"];
				}
				if(alwaysProtected.includes(methodName) || _passwordProtected.includes(methodName)) {
					if(password == null) {
						return {
							proceed: false,
							error: "tEnvoyNaClKey Fatal Error: Key is password-protected for method " + methodName + ", and no password was specified"
						};
					} else if(!_tEnvoy.util.compareConstant(password, _getPassword())) {
						return {
							proceed: false,
							error: "tEnvoyNaClKey Fatal Error: Key is password-protected for method " + methodName + ", and an incorrect password was specified."
						};
					} else {
						return {
							proceed: true
						};
					}
				} else {
					return {
						proceed: true
					};
				}
			}
		}
	}
}

function tEnvoyNaClSigningKey(key, type = "secret", password = null, passwordProtected = [], tEnvoy = TogaTech.tEnvoy) {
	let _key;
	let _nonce;
	let _password;
	let _passwordProtected;
	let _type;
	let _assertPassword;
	let _getKey;
	let _setKey;
	let _getPassword;
	let _setPassword;
	let _tEnvoy = tEnvoy;
	let _nacl = _tEnvoy.core.nacl;

	this.destroy = (password = null) => {
		let assertion = _assertPassword("destroy", password);
		if(assertion.proceed) {
			if(_key != null) {
				for(let i = 0; i < _key.length; i++) {
					_key[i] = 0;
				}
			}
			if(_password != null) {
				for(let i = 0; i < _password.length; i++) {
					_password[i] = 0;
				}
			}
			for(method in this) {
				delete this[method];
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.toString = () => {
		return `[tEnvoyNaClSigningKey ${_type}]`;
	}
	
	this.getType = () => {
		return _type;
	}
	
	this.getPasswordProtected = () => {
		return _tEnvoy.util.arrayDeepCopy(_passwordProtected);
	}

	this.setPasswordProtected = (passwordProtected, password = null) => {
		let assertion = _assertPassword("setPasswordProtected", password);
		if(assertion.proceed) {
			_passwordProtected = [];
			let protectable = [];
			if(_type == "private") {
				protectable = ["destroy", "getPublic", "sign", "verify"];
			} else if(_type == "public") {
				protectable = ["destroy", "verify"];
			}
			if(passwordProtected == null) {
				passwordProtected = [];
			}
			for(let i = 0; i < passwordProtected.length; i++) {
				if(protectable.includes(passwordProtected[i])) {
					_passwordProtected.push(passwordProtected[i]);
				}
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.getPrivate = (password = null) => {
		let assertion = _assertPassword("getPrivate", password);
		if(assertion.proceed) {
			if(_type == "private") {
				if(_getPassword() == null) {
					return _tEnvoy.util.arrayDeepCopy(_getKey());
				} else {
					let decryptionKey = new tEnvoyNaClKey(_getPassword(), "secret", null, [], _tEnvoy);
					let decrypted = decryptionKey.decrypt(_getKey());
					decryptionKey.destroy();
					if(_tEnvoy.util.bytesToHex(decrypted.nonce) == _tEnvoy.util.bytesToHex(_nonce)) {
						return decrypted.message;
					} else {
						throw "tEnvoyNaClSigningKey Fatal Error: The encrypted key was tampered with, and the nonce is invalid.";
					}
				}
			} else {
				throw "tEnvoyNaClSigningKey Fatal Error: Key does not have a private component.";
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.setPrivate = (privateKey, password = null) => {
		let assertion = _assertPassword("setPrivate", password);
		if(assertion.proceed) {
			if(privateKey == null) {
				throw "tEnvoyNaClSigningKey Fatal Error: argument privateKey of method setPrivate is required and does not have a default value.";
			}
			privateKey = _tEnvoy.util.mixedToUint8Array(privateKey, false);
			if(privateKey.length != 32) {
				throw "tEnvoyNaClSigningKey Fatal Error: argument privateKey of method setPrivate is invalid, length should be 32 (was " + privateKey.length + ").";
			}
			if(_type == "private") {
				if(_getPassword() == null) {
					_setKey(privateKey);
				} else {
					_nonce = _nacl.randomBytes(12);
					let encryptionKey = new tEnvoyNaClKey(_getPassword(), "secret", null, [], _tEnvoy);
					_setKey(encryptionKey.encrypt(privateKey, _nonce));
					encryptionKey.destroy();
				}
			} else {
				throw "tEnvoyNaClSigningKey Fatal Error: Key does not have a private component.";
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.getPublic = (password = null) => {
		let assertion = _assertPassword("getPublic", password);
		if(assertion.proceed) {
			if(_type == "private") {
				return _nacl.sign.keyPair.fromSecretKey(this.getPrivate(_getPassword())).publicKey;
			} else if(_type == "public") {
				if(_getPassword() == null) {
					return _tEnvoy.util.arrayDeepCopy(_getKey());
				} else {
					let decryptionKey = new tEnvoyNaClKey(_getPassword(), "secret", null, [], _tEnvoy);
					let decrypted = decryptionKey.decrypt(_getKey());
					decryptionKey.destroy();
					if(_tEnvoy.util.bytesToHex(decrypted.nonce) == _tEnvoy.util.bytesToHex(_nonce)) {
						return decrypted.message;
					} else {
						throw "tEnvoyNaClSigningKey Fatal Error: The encrypted key was tampered with, and the nonce is invalid.";
					}
				}
			} else {
				throw "tEnvoyNaClSigningKey Fatal Error: Key does not have a public component.";
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.setPublic = (publicKey, password = null) => {
		let assertion = _assertPassword("setPublic", password);
		if(assertion.proceed) {
			if(publicKey == null) {
				throw "tEnvoyNaClSigningKey Fatal Error: argument publicKey of method setPublic is required and does not have a default value.";
			}
			publicKey = _tEnvoy.util.mixedToUint8Array(publicKey, false);
			if(publicKey.length != 32) {
				throw "tEnvoyNaClSigningKey Fatal Error: argument publicKey of method setPublic is invalid, length should be 32 (was " + publicKey.length + ").";
			}
			if(_type == "private") {
				throw "tEnvoyNaClSigningKey Fatal Error: Key has a public component that depends on the private component.";
			} else if(_type == "public") {
				if(_getPassword() == null) {
					_setKey(publicKey);
				} else {
					_nonce = _nacl.randomBytes(12);
					let encryptionKey = new tEnvoyNaClKey(_getPassword(), "secret", null, [], _tEnvoy);
					_setKey(encryptionKey.encrypt(publicKey, _nonce));
					encryptionKey.destroy();
				}
			} else {
				throw "tEnvoyNaClSigningKey Fatal Error: Key does not have a public component.";
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.sign = (message, password = null) => {
		let assertion = _assertPassword("sign", password);
		if(assertion.proceed) {
			if(message == null) {
				throw "tEnvoyNaClSigningKey Fatal Error: argument message of method sign is required and does not have a default value.";
			}
			if(_type == "private") {
				message = _tEnvoy.util.pack(message);
				let hashed = _tEnvoy.util.bytesToHex(_nacl.hash(message)); // sha512 hash
				return {
					signature: hashed + "::" + _tEnvoy.util.bytesToHex(_nacl.sign.detached(_nacl.hash(message), this.getPrivate(_getPassword()))),
					hash: hashed
				};
			} else {
				throw "tEnvoyNaClSigningKey Fatal Error: Key does not have a private component.";
			}
		} else {
			throw assertion.error;
		}
	}
	
	this.verify = (signed, password = null) => {
		let assertion = _assertPassword("verify", password);
		if(assertion.proceed) {
			if(signed == null) {
				throw "tEnvoyNaClSigningKey Fatal Error: argument signed of method verify is required and does not have a default value.";
			}
			if(signed.split("::").length != 2) {
				throw "tEnvoyNaClSigningKey Fatal Error: Invalid signature.";
			}
			let hash = _tEnvoy.util.hexToBytes(signed.split("::")[0]);
			let signature = _tEnvoy.util.hexToBytes(signed.split("::")[1]);
			return {
				verified: _nacl.sign.detached.verify(hash, signature, this.getPublic(_getPassword())),
				hash: signed.split("::")[0]
			};
		} else {
			throw assertion.error;
		}
	}
	
	this.verifyWithMessage = (signed, message, password = null) => {
		if(signed == null) {
			throw "tEnvoyNaClSigningKey Fatal Error: argument signed of method verifyWithMessage is required and does not have a default value.";
		}
		if(message == null) {
			throw "tEnvoyNaClSigningKey Fatal Error: argument message of method verifyWithMessage is required and does not have a default value.";
		}
		if(signed.split("::").length != 2) {
			throw "tEnvoyNaClSigningKey Fatal Error: Invalid signature.";
		}
		let hash = _tEnvoy.util.hexToBytes(signed.split("::")[0]);
		return this.verify(signed, password) && _tEnvoy.util.bytesToHex(_nacl.hash(_tEnvoy.util.pack(message))) == _tEnvoy.util.bytesToHex(hash);
	}
	
	this.toPublic = (password = null) => {
		return new tEnvoyNaClSigningKey(this.getPublic(password), "public", _getPassword(), _passwordProtected, _tEnvoy);
	}
	
	if(!["public", "private"].includes(type)) {
		throw "tEnvoyNaClSigningKey Fatal Error: argument type of method constructor is invalid.";
	} else {
		_getKey = () => {
			return _tEnvoy.util.unpack(_key);
		}

		_setKey = (newKey) => {
			_key = _tEnvoy.util.pack(newKey);
		}

		_getPassword = () => {
			return _tEnvoy.util.unpack(_password);
		}

		_setPassword = (newPassword) => {
			_password = _tEnvoy.util.pack(newPassword);
		}

		_setPassword(password);
		if(password == null) {
			_setKey(key);
		} else {
			_nonce = _nacl.randomBytes(12);
			let encryptionKey = new tEnvoyNaClKey(password, "secret", null, [], _tEnvoy);
			_setKey(encryptionKey.encrypt(key, _nonce));
			encryptionKey.destroy();
		}
		_type = type;
		_passwordProtected = [];
		let protectable = [];
		if(_type == "private") {
			protectable = ["destroy", "getPublic", "sign", "verify"];
		} else if(_type == "public") {
			protectable = ["destroy", "verify"];
		}
		if(passwordProtected == null) {
			passwordProtected = [];
		}
		for(let i = 0; i < passwordProtected.length; i++) {
			if(protectable.includes(passwordProtected[i])) {
				_passwordProtected.push(passwordProtected[i]);
			}
		}
		_assertPassword = (methodName, password = null) => {
			if(_getPassword() == null) {
				return {
					proceed: true
				};
			} else {
				let alwaysProtected;
				if(_type == "private") {
					alwaysProtected = ["getPrivate", "setPrivate", "setPasswordProtected"];
				} else if(_type == "public") {
					alwaysProtected = ["getPublic", "setPublic", "setPasswordProtected"];
				}
				if(alwaysProtected.includes(methodName) || _passwordProtected.includes(methodName)) {
					if(password == null) {
						return {
							proceed: false,
							error: "tEnvoyNaClSigningKey Fatal Error: Key is password-protected for method " + methodName + ", and no password was specified."
						};
					} else if(!_tEnvoy.util.compareConstant(password, _getPassword())) {
						return {
							proceed: false,
							error: "tEnvoyNaClSigningKey Fatal Error: Key is password-protected for method " + methodName + ", and an incorrect password was specified."
						}
					} else {
						return {
							proceed: true
						};
					}
				} else {
					return {
						proceed: true
					};
				}
			}
		}
	}
}


TogaTech.tEnvoy = new tEnvoy(openpgp, nacl, sha256);

module.exports = {tEnvoy, tEnvoyPGPKey, tEnvoyNaClKey, tEnvoyNaClSigningKey};