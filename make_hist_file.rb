File.open('.hhush.histfile', 'w') do |file|
  1000.times do |i|
    file.write("#{i}\n")
  end
end