require 'nokogiri'
require 'open-uri'

while true
doc = Nokogiri::HTML(open("http://www.last.fm/user/iamelliot"))

songcell= doc.css('.subjectCell.highlight').first

song_info  = ["","",""]
index = 0
atags = songcell.css('a')


atags.each do |atag|
	song_info[index] = atag.text
	song_info[index] << "                        "
	song_info[index] = song_info_index[0..25]
	index = index + 1

end
system("echo #{song_info[0]} #{song_info[1]} #{song_info[1]}")
system('sleep 5')
end
