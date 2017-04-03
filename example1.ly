\paper {
  top-system-spacing #'basic-distance = #10
  score-system-spacing #'basic-distance = #20
  system-system-spacing #'basic-distance = #20
  last-bottom-spacing #'basic-distance = #10
}

global = {
  \key c \major
  \time 4/4
}

sopMusic = \relative c'' {
        c b c c a d b c g a a b c d b c
}
altoMusic = \relative c' {
        g' g f e e a f e e e d g g a g g
}
tenorMusic = \relative c' {
        e e c c c d d c b c a d e d d e
}
bassMusic = \relative c {
        c e a g, a' f d g e e f g c, f g c
}
\score {
  <<
    \new PianoStaff <<
      \new Staff <<
        \set Staff.printPartCombineTexts = ##f
        \partcombine
        << \global \sopMusic >>
        << \global \altoMusic >>
      >>
      \new Staff <<
        \clef bass
        \set Staff.printPartCombineTexts = ##f
        \partcombine
        << \global \tenorMusic >>
        << \global \bassMusic >>
      >>
      \new FiguredBass \figuremode {
         \set figuredBassAlterationDirection = #RIGHT
         \set figuredBassPlusDirection = #RIGHT
       \override BassFigureAlignment.stacking-dir = #DOWN
             <1>4 <3>4 <4>4 <1>4 <6>4 <2>4 <7>4 <1>4 <3>4 <6>4 <2>4 <5>4 <1>4 <2>4 <5>4 <1>4
      }
    >>
  >>
  \layout{}
\midi{}
}
