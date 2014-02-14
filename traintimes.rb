#!/usr/bin/ruby
#
Encoding.default_external = Encoding::UTF_8
Encoding.default_internal = Encoding::UTF_8
require 'nokogiri'
require 'open-uri'
doc = Nokogiri::HTML(open("http://www.realtimetrains.co.uk/search/advanced/HAY"))


trains= []

index = 0
doc.css('.call_public').each do |row|



  time= row.css('.time')[0].text
  dest = row.css('.location')[1].text
  expected =row.css('.time')[1].text

  time = "#{time}                        "[0..3]
  dest = "#{dest}                        "[0..12]
  expected = "#{expected}                        "[0..3]


  trains[index] =  "#{time} #{dest} #{expected}"[0..23]

    index =index + 1
    if(index == 3)
      system("writetosign \"#{trains[0]}\" \"#{trains[1]}\" \"#{trains[2]}\"  ")
    end
  end


