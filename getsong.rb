#!/usr/bin/ruby
#
require 'nokogiri'
require 'open-uri'

while true

  # Now playing
  system("writetosign \"                         \" \"  NOW PLAYING          \"  \"                         \"  \"                         \" ")

  doc = Nokogiri::HTML(open("http://www.last.fm/user/iamelliot"))
  if doc.css('.subjectCell.highlight').length > 0
    songcell= doc.css('.subjectCell.highlight').first

    song_info = ["", "", ""]
    index = 0
    atags = songcell.css('a')


    atags.each do |atag|
      song_info[index] << atag.text
      song_info[index] << "                        "
      song_info[index] = song_info[index][0..25]
      index = index + 1

    end
    system("writetosign #{song_info[0]} #{song_info[1]} #{song_info[2]}")
    sleep 30

  else
    system("writetosign \"                         \" \"       NOTHING          \"  \"                         \"  \"                         \" ")

    sleep 5

  end

  # Clock


  system("writetosign \"                         \" \"                         \"  \"                         \"  \"                         \" ")

  120.times do
  datetext = Time.now.strftime "%a %d %b        "
  timetext = Time.now.strftime "%H:%M:%S"

    system("writetosign \"#{datetext}\" \"                      \"  \"                 #{timetext}\" ")

    sleep 0.25
  end


  system("writetosign \"ActionRocket          \" \"                         \"  \"                         \"  \"                         \" ")



  # Tweet ?



  sleep 20
  system("writetosign \"       FARRINGDON        \" \"     Departures  \"  \"                         \"  \"                         \" ")


  system("traintimes")
  sleep 30

end
