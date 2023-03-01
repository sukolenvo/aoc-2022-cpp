//
// Created by object on 29/01/23.
//

#ifndef AOC_2022_CPP_LEVEL6_HPP
#define AOC_2022_CPP_LEVEL6_HPP

#include <algorithm>
#include <map>
#include <exception>
#include <iostream>

#include "common.hpp"

namespace level6 {

auto searchMarker(const std::string_view &input, auto markerLength) {
  std::map<char, int> lastOccurrences;
  int uniqueLength = 0;
  int i = 0;
  for (const auto &c :input) {
    if (lastOccurrences.contains(c)) {
      auto distance = i - lastOccurrences[c];
      uniqueLength = std::min(distance, uniqueLength + 1);
    } else {
      ++uniqueLength;
    }
    lastOccurrences[c] = i++;
    if (uniqueLength == markerLength) {
      return i;
    }
  }
  throw std::invalid_argument("cannot find datagram marker in the requested string");
}

const auto task_input = "pfptpztzfznzzznszzfgzgqgbgzgmzggwlwnlngnddjttzwtwmttlrrlqqzczpzhppmjmnjnfjnjjjprpfpnfpnpznzbbnwbbdsbswwwwszzzcmcttbftffczctztffppcvccbdbhbtbcbhbrbnrbrzbztbblvlvqvcqvcvcrcprrmffhfwfjwjrjtttvqqsttcwtthwthhqssbbqhhcbhbhqqwsqspqspprbrsrmmvhvvlsvspvpddbvdbdhhgshggfbgbrggzszsnshsvhsvhhnfnhfnhhbzzthtstdssdsrdsrdrtddsrdrcrgcgdcggdbbtgbgzbzwwnlngggspggspspjjfqflqqttjbbnffszfzjjlwjwhjwjccpllzztrzttdpdbbcnntwnnctnnrrgbrbvbccvzvlvnllvrvnnvmmtrmrddjdqdrqrmmvssgllsjsrrwggvcccgwwgddzldzldzdttqrrtggdngdgfdgfgzztdtthrttszzgjglldzdhdphppjfjfzfpfsflfggpwwwjdjssqdqmdmdpmmvbvfvpfpfwfpwfppbgbwgbbpmpwpzplpjpttnwnlnppcvchhmwwmllwppwspwspswpphhdhfhccwpcplpzzjwjssfrsssgnglnglnglngncgcvvpddrcrjrssznnjllslrlmllglpggwhhllzqzssvtsvvbpphpthphdhphnhdhdqdbbmcmqcmcmddtwtqqsbbzddtzdzrzmmfccdttlvljlffzczmzrrtzzrbrllfnfmfnnrcnnctcnnncbcrbccwcnwcnnrbnrrpnprrtwwvcwchwwcbwwwnpnvvgtvvqmvvhttrnnvjjtwtfwtwrrbpbmbmzmpphwphwwqwgwlwclwccjvccsllhhrtrstthvvfjjcvcdcbdcbczcttjbttwtvvvzjzgzwgwjjsnnvrrlbrrvnvdvhvchvvglvvpssdbsszddsqsspvpbpjjgghvvlwvlvggpjjmcjmmmhjmmrgrsgrsggchhcmmqsmsttlslqqmjjbpbrrcnrnbbvqvsvtvftvtmmqzzsnscsffnrnmrmnnszsllsrrrzrszsbbchccsrsmshhzrhrrdlrrjfmlrfhvqqvmpbrntgcqqsqvjmtctflbffddfbsjvzsfdwblprszhfvltwtcfsbdlwjgsmlcrvgstjqtrtnqzbmrmgqnscqjdfnbppcdgcsstwdmdvphsqmrfmzwntjgjjvcdgbhfjqlzglgjdsdlhwwrmfqcfsvhwwfmvprpnmjppvwzjwmddtndspzqjqrpbpnrjfwqfvbtqrgngcbjvhnfbtslcpppbsfhbcmwgpccftwhnbvdrzqdtwnrtjcdlnlmhvlzvljwrzgtfjrpgzjvggtpsvcdgtsvhdzvtfbwmnptfmllgcvfmmgvpbrgnhcnpwltqmjmltsbpzmfrttsmjqwhncvtqrsmcpsnrqzmwftbltllbhzhdfzmfgbvdtgwpvngsffjmwhfhmccfrjgqcqngzlnqvsgrcdzbsmjbmflwvhjldlrdvjrmgvjvpcczdhczpbtwphvhqmhcnljbwnzqwmbctffmctlcmhzcnvprdhtzdvgbhlnnjqzcwcsrgzjjlszssnplwqjlczvftmnbnmdpbjnctnslhgsjswqjwvprdstvbstlnnwwgvsffwmprjrlfccmtgvqbghvhcngwwtzwbwcdmrfstwhtfghgvzbfgtwjglcllwrhgdzptvrrbdhbscjhmtswshjmrsbpzstwmhrwwwncbbmjnjjlzrpdrzfvstbltszvlhcqbcpgbwtzzslsrljmhmtlcvzdbszvnjhrswrjrmfpsfpplwlrsnrpnjngmhwpwqcmtslhbmlsmjhcgmzznftmhvtmzlvmcwnbqtcntqghrqcsztsgzrnmrlvrnhtpmstdflpztmwltvgppttfwhhzzgrffjchswhbljvcjwvvnqnvdvjpsclhwsrtczvjmtcsnwvnwtdllphmrthddfvcjwvggqltmhglllmqzjsbjwgdqwzzmjnrmpbqplgzjgzcdqmtsntprdwwjwthcbsghqqspszndgqdmlzdlzwfcghtbhcqpbpmnfgqzmhtnttvjttvzhllsjvmmcmmppcgssjhnzqwpzdbtmrzsfbvgmgbtbwjrzvdlmgjpzltfmcclpltsszpqbllrwbwsnbhhvfwphrcpdvbjhgmgpphrdpcmjvfsjzrqldlqthwsztzcgttdnzcsbnszcsvmcspddlmwjttggdmlpqrdrfmwfzpdbnrwtmwssvbwtmzhndmhzwtlgdwpbrzghmlbszswqlpzldbvswjgtvjvmtjwdggfsbggbwhpwjdmflhmsgtbzrtbvlpqqmpcrbhflnfmwwsvdsgnnznfrqhqgqfgdfzcdqrtdftsntpbcclhncqjjwvszmssswnscwjlpfvdvltgcmqqttnfvbptbbmlrvrwwfbwwbvlrdrfmscqwdvdjgdrghwfjsttvwngzttzzsmzqnvzdfsvrbrcwtmmjdvnzjzdsnzgtszzcwdphnjmspmdsrqwgdwlzrgghcchpbltmwnjrbhqhzdqqmbrpggjjwnqfnnsqsfzbwqjsfprvrvfwbqvhgpjvqzplnhtqszqrtsvtbptfvzmvjhshbtmqbmqrrwplzphvdvhttlmftdwltqssstzlvnslzhnmjdlsbdprbgpjvcdtcfchzqqqcnngbrmntjwfbvcdcfgbcpnvcbbcvhqfzpsmgbcvrqvjlqlqnvvzdgphfpgtrpbbwztvqjgdpnpwbffdgqzmqvgblwzmdrhwdprhcqppcggrldhcdztnhspclfcwttnqslnzvvshcwgfztvscvztdrprvnlmfsgcpfdmfjgblnhrbsmjrjdzjwvwmlllvscsvfqvhdsdljrqphcvvtcttbwvnwwzwshdcfdqnjszltmddzjgmqgvpjzpzssrmfsrgjhvqtlhsfnndnqhpbcnltmdvlhfwqcmwnqbhsfqqwnfnnfjjbsqmcdrrvlfztdprnmjfhlvcdbjtczbrpljmpcwvchdwrqbwggjnrlhcdgzfwjzzjgfnbpwbpvswqdpcrthwfcffgztrjqntczfcbsrrtrjrwgbbgjshtzvjjlqqtsbgmpsttqjqwgmmbzhshqvvrcgbdsqmtqlrgjbnvbrpzdrgqnzstfdcvdnjhcnjblmsqtfvstlptgrczhbgpllpqwfdmthgjlltmlnltzpvjvjfgtrzslsptlfplgrgpjsbhbbbwlljfdjnhqcndlprfbwpvddndpnqwqccgbqmwlrffpzjpqclwcrgjgljwzpppwltcwdqdchghfnwbhrjndjsvlqnnmlrjfgfpnvgmlhbgnhnztpjzdmltfmjtzclsbspvhfngtjmzwrwmprdfplzzwfrdnbmbgvjlczcdvmpfmtqmzjrpfhjwwzmtnzmptwnhtlbndcpshqrqqrpccqpnvnqqdprvccmdmrsbptdhrhlpcptgfsfwphfpvbcrlnbrtgwcpgjclhhvpjhcwcgghlzbmpbswgtzqhmlwdfrrdfvnbhlqhvhnfjfndlqgrvhwnnnccvgdfqtwlmbwcpdtgscfpvmbdtcdmmgqrfjvnhngqsdtzhlbjwrrcrjfswwrgbhznlwhcjlsfprbqqcqmbdjhgjmmtqmjpldgqvptqcwjmlrjtjwdfbbvhpsnmfvdwnrntqzhfgfmrtgwgddpqvvdjqvrdwdwrsbjlbrmrjjbbpjpqgsjdzfjcrsnbmtmrstcrztzhgswgghwbfltdsvrcqvvjtmjwznnnwtsmshvbbpzwltrjpmbgsbqwphmwlhgpltsgjmgbdfrlhcbfjnvpvdwzccgdhswtgplcqnsjdwfbhbbpssvfrjbzmcphzjdncjgsvrcrplhqpnwdgfvrjqgfshdwrqjdvjmggtnnghqrccgddnzndcgpgpghtvrpwftfpttvgwqqcjbvnmqzlshdrdj";

void run() {
  const int packetMarkerLength = 4;
  const int messageMarkerLength = 14;
  std::cout << searchMarker(task_input, packetMarkerLength) << '\n';
  std::cout << searchMarker(task_input, messageMarkerLength) << '\n';
}
}

#endif// AOC_2022_CPP_LEVEL6_HPP
