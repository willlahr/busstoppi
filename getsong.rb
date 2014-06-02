#!/usr/bin/ruby
#
require 'nokogiri'
require 'open-uri'

while true

  begin
  # Now playing

  doc = Nokogiri::HTML(open("http://www.last.fm/user/actionrocket"))
  if doc.css('.subjectCell.highlight').length > 0
    system("writetosign \"                         \" \"       NOW PLAYING      \"  \"                         \"  \"                         \" ")
    sleep 5

    songcell= doc.css('.subjectCell.highlight').first

    song_info = ["", "", ""]
    index = 0
    atags = songcell.css('a')
    

    atags.each do |atag|
      song_info[index] << atag.text
      song_info[index] << "                        "
      song_info[index] = song_info[index][0..23]
      index = index + 1

    end
    system("writetosign \"#{song_info[0].upcase}\" \"#{song_info[1].upcase}\" \"#{song_info[2].upcase}\"")
    sleep 15
  end

  # Clock


  system("writetosign \"                         \" \"                         \"  \"                         \"  \"                         \" ")

  lasttime = ""

  60.times do
  datetext = Time.now.strftime "%a %d %b        "
  timetext = Time.now.strftime "%H:%M:%S"

  if timetext != lasttime
    system("nice -n 19 writetosign \"#{datetext}\" \"                      \"  \"                #{timetext}\" ")
  end
    lasttime = timetext
    sleep 0.25
  end

  # Taxi status
  doc = Nokogiri::HTML(open("http://ef.emailcms.net/status"))
  song_info[0]= "TAXI SERVERS          #{doc.css('.servers_running').text}            "[0..23]
  song_info[1]= "QUEUE ITEMS           #{doc.css('.queue_items').text}            "[0..23]
  song_info[2]= "QUEUE PROCESSORS     #{doc.css('.queue_processors_running').text}            "[0..23]



  system("writetosign \"#{song_info[0]}\" \"#{song_info[1]}\" \"#{song_info[2]}\"")

  doc = Nokogiri::HTML(open("http://ef.emailcms.net/status"))
  song_info[0]= "USERS         #{doc.css('.content-items').text}            "[0..23]
  song_info[1]= "VERSIONS      #{doc.css('.versions').text}            "[0..23]
  song_info[2]= "CONTENT ITEMS #{doc.css('.content-items').text}            "[0..23]



  # Tweet ?

  sleep 10


  system("nice -n 19 traintimes")
  sleep 10
  rescue

    end

end
