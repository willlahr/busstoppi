#!/usr/bin/ruby
#
Encoding.default_external = Encoding::UTF_8
Encoding.default_internal = Encoding::UTF_8
require 'nokogiri'
require 'open-uri'

year = Time.now.strftime("%Y")
month = Time.now.strftime("%m")
day = Time.now.strftime("%d")


hours_minutes_now = (Time.now + 600).strftime("%H%M")

hours_now = Integer((Time.now).strftime("%H"))

if(hours_now >14)

  system("writetosign \"       FARRINGDON        \" \"       departures    \"  \"                         \"  \"                         \" ")


  url = "http://www.realtimetrains.co.uk/search/advanced/ZFD/to/SUO/#{year}/#{month}/#{day}/#{hours_minutes_now}-2359"
puts url

doc = Nokogiri::HTML(open(url))



trains = []

index = 0
doc.css('.call_public').each do |row|



  time= row.css('.time')[0].text
  dest = row.css('.location')[1].text
  expected =row.css('.realtime')[1].text

  time = "#{time}                        "[0..3]
  dest = "#{dest}                        "[0..11]
  expected = "#{expected}                        "[0..3]

  if(time == expected)
    expected = "OnTime"
  else
    expected = "  #{expected}"

  end
  trains[index] =  "#{time} #{dest} #{expected}    "[0..24]

    index =index + 1
    if(index == 3)
      system("writetosign \"#{trains[0]}\" \"#{trains[1]}\" \"#{trains[2]}\"  ")
    end
  end
end


