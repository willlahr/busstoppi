#!/usr/bin/ruby
#
Encoding.default_external = Encoding::UTF_8
Encoding.default_internal = Encoding::UTF_8
require 'nokogiri'
require 'open-uri'
doc = Nokogiri::HTML(open("http://traintimes.org.uk/live/hayesandharlington"))


trains= []

index = 0
doc.css('tr').each do |row|

  cells = row.css('td')

  if cells.length > 2
  time= "#{cells[0].text[0..4]}"

  dest_text = cells[1].text
  dest_array= dest_text.split "\2014"
  dest = dest_array[0]

  dest = "#{dest}            "[0..12]

  expected = "FUCKED"

  trains[index] =  "#{time} #{dest}#{expected}"[0..24]

    index =index + 1
    if(index == 3)
      system("writetosign \"#{trains[0]}\" \"#{trains[1]}\" \"#{trains[2]}\"  ")
    end
  end


  end

