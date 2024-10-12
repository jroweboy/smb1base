object FormBrush: TFormBrush
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Brush mask toolbox'
  ClientHeight = 154
  ClientWidth = 280
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PopupMode = pmExplicit
  Position = poDesigned
  ScreenSnap = True
  SnapBuffer = 20
  OnActivate = FormActivate
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnKeyUp = FormKeyUp
  PixelsPerInch = 96
  TextHeight = 13
  object PaintBoxMask: TPaintBox
    Left = 5
    Top = 5
    Width = 84
    Height = 84
    OnMouseDown = PaintBoxMaskMouseDown
    OnMouseEnter = PaintBoxMaskMouseEnter
    OnMouseLeave = PaintBoxMaskMouseLeave
    OnMouseMove = PaintBoxMaskMouseMove
    OnMouseUp = PaintBoxMaskMouseUp
    OnPaint = PaintBoxMaskPaint
  end
  object SpeedButton24: TSpeedButton
    Left = 26
    Top = 130
    Width = 20
    Height = 19
    Caption = 'D'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = 'Wingdings 3'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton24Click
    OnMouseEnter = SpeedButton24MouseEnter
    OnMouseLeave = SpeedButton24MouseLeave
  end
  object SpeedButton25: TSpeedButton
    Left = 5
    Top = 130
    Width = 20
    Height = 19
    Caption = 'E'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = 'Wingdings 3'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton25Click
    OnMouseEnter = SpeedButton25MouseEnter
    OnMouseLeave = SpeedButton24MouseLeave
  end
  object SpeedButton26: TSpeedButton
    Left = 114
    Top = 130
    Width = 20
    Height = 19
    Caption = 'P'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = 'Wingdings 3'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton26Click
    OnMouseEnter = SpeedButton26MouseEnter
    OnMouseLeave = SpeedButton24MouseLeave
  end
  object SpeedButton27: TSpeedButton
    Left = 93
    Top = 130
    Width = 20
    Height = 19
    Caption = 'Q'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = 'Wingdings 3'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton27Click
    OnMouseEnter = SpeedButton27MouseEnter
    OnMouseLeave = SpeedButton24MouseLeave
  end
  object SpeedButton32: TSpeedButton
    Left = 137
    Top = 54
    Width = 66
    Height = 16
    Caption = 'presets..'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton32Click
    OnMouseEnter = SpeedButton32MouseEnter
    OnMouseLeave = SpeedButton24MouseLeave
  end
  object SpeedButton33: TSpeedButton
    Left = 99
    Top = 54
    Width = 34
    Height = 16
    Caption = 'set'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton33Click
    OnMouseLeave = SpeedButton24MouseLeave
  end
  object SpeedButton34: TSpeedButton
    Left = 99
    Top = 71
    Width = 34
    Height = 16
    Caption = 'clear'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton34Click
    OnMouseLeave = SpeedButton24MouseLeave
  end
  object SpeedButton35: TSpeedButton
    Left = 99
    Top = 88
    Width = 34
    Height = 16
    Caption = 'invert'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton35Click
    OnMouseLeave = SpeedButton24MouseLeave
  end
  object SpeedButton36: TSpeedButton
    Left = 49
    Top = 130
    Width = 20
    Height = 19
    Caption = '2'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Wingdings 3'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton36Click
    OnMouseEnter = SpeedButton25MouseEnter
    OnMouseLeave = SpeedButton24MouseLeave
  end
  object SpeedButton37: TSpeedButton
    Left = 70
    Top = 130
    Width = 20
    Height = 19
    Caption = '1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Wingdings 3'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton37Click
    OnMouseEnter = SpeedButton25MouseEnter
    OnMouseLeave = SpeedButton24MouseLeave
  end
  object LabelXY: TLabel
    Left = 8
    Top = 116
    Width = 16
    Height = 11
    Caption = 'x,y:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object SpeedButton42: TSpeedButton
    Left = 137
    Top = 71
    Width = 66
    Height = 16
    Caption = 'make brush..'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton42Click
    OnMouseLeave = SpeedButton24MouseLeave
  end
  object SpeedButton43: TSpeedButton
    Left = 208
    Top = 54
    Width = 35
    Height = 16
    Caption = 'save..'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton43Click
    OnMouseEnter = SpeedButton32MouseEnter
    OnMouseLeave = SpeedButton24MouseLeave
  end
  object SpeedButton44: TSpeedButton
    Left = 244
    Top = 54
    Width = 35
    Height = 16
    Caption = 'load'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton44Click
    OnMouseEnter = SpeedButton32MouseEnter
    OnMouseLeave = SpeedButton24MouseLeave
  end
  object GroupBox1: TGroupBox
    Left = 98
    Top = 0
    Width = 182
    Height = 52
    Caption = 'Brush'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object SpeedButton3: TSpeedButton
      Left = 4
      Top = 14
      Width = 24
      Height = 16
      GroupIndex = 1
      Caption = '2a'
      OnClick = SpeedButton3Click
      OnMouseEnter = SpeedButton3MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object SpeedButton4: TSpeedButton
      Tag = 1
      Left = 29
      Top = 14
      Width = 24
      Height = 16
      GroupIndex = 1
      Caption = '3a'
      OnClick = SpeedButton3Click
      OnMouseEnter = SpeedButton3MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object SpeedButton5: TSpeedButton
      Tag = 2
      Left = 54
      Top = 14
      Width = 24
      Height = 16
      GroupIndex = 1
      Caption = '4a'
      OnClick = SpeedButton3Click
      OnMouseEnter = SpeedButton3MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object SpeedButton6: TSpeedButton
      Tag = 7
      Left = 4
      Top = 31
      Width = 24
      Height = 16
      GroupIndex = 1
      Caption = '2b'
      OnClick = SpeedButton3Click
      OnMouseEnter = SpeedButton3MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object SpeedButton7: TSpeedButton
      Tag = 8
      Left = 29
      Top = 31
      Width = 24
      Height = 16
      GroupIndex = 1
      Caption = '3b'
      OnClick = SpeedButton3Click
      OnMouseEnter = SpeedButton3MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object SpeedButton8: TSpeedButton
      Tag = 9
      Left = 54
      Top = 31
      Width = 24
      Height = 16
      GroupIndex = 1
      Caption = '4b'
      OnClick = SpeedButton3Click
      OnMouseEnter = SpeedButton3MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object SpeedButton9: TSpeedButton
      Tag = 3
      Left = 79
      Top = 14
      Width = 24
      Height = 16
      GroupIndex = 1
      Caption = '5a'
      OnClick = SpeedButton3Click
      OnMouseEnter = SpeedButton3MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object SpeedButton10: TSpeedButton
      Tag = 10
      Left = 79
      Top = 31
      Width = 24
      Height = 16
      GroupIndex = 1
      Caption = '5b'
      OnClick = SpeedButton3Click
      OnMouseEnter = SpeedButton3MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object SpeedButton11: TSpeedButton
      Tag = 4
      Left = 104
      Top = 14
      Width = 24
      Height = 16
      GroupIndex = 1
      Caption = '6a'
      OnClick = SpeedButton3Click
      OnMouseEnter = SpeedButton3MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object SpeedButton12: TSpeedButton
      Tag = 5
      Left = 129
      Top = 14
      Width = 24
      Height = 16
      GroupIndex = 1
      Caption = '7a'
      OnClick = SpeedButton3Click
      OnMouseEnter = SpeedButton3MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object SpeedButton13: TSpeedButton
      Tag = 6
      Left = 154
      Top = 14
      Width = 24
      Height = 16
      GroupIndex = 1
      Caption = '8a'
      OnClick = SpeedButton3Click
      OnMouseEnter = SpeedButton3MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object SpeedButton14: TSpeedButton
      Tag = 11
      Left = 104
      Top = 31
      Width = 24
      Height = 16
      GroupIndex = 1
      Caption = '6b'
      OnClick = SpeedButton3Click
      OnMouseEnter = SpeedButton3MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object SpeedButton15: TSpeedButton
      Tag = 12
      Left = 129
      Top = 31
      Width = 24
      Height = 16
      GroupIndex = 1
      Caption = '7b'
      OnClick = SpeedButton3Click
      OnMouseEnter = SpeedButton3MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object SpeedButton16: TSpeedButton
      Tag = 13
      Left = 154
      Top = 31
      Width = 24
      Height = 16
      GroupIndex = 1
      Caption = '8b'
      OnClick = SpeedButton3Click
      OnMouseEnter = SpeedButton3MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
  end
  object GroupBox2: TGroupBox
    Left = 136
    Top = 87
    Width = 69
    Height = 66
    Caption = 'align cursor'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnMouseEnter = GroupBox2MouseEnter
    OnMouseLeave = SpeedButton24MouseLeave
    object SpeedButton1: TSpeedButton
      Left = 3
      Top = 13
      Width = 20
      Height = 16
      GroupIndex = 1
      Caption = 'j'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton23Click
    end
    object SpeedButton2: TSpeedButton
      Tag = 4
      Left = 24
      Top = 30
      Width = 20
      Height = 16
      GroupIndex = 1
      Caption = 'c'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton23Click
    end
    object SpeedButton17: TSpeedButton
      Tag = 1
      Left = 24
      Top = 13
      Width = 20
      Height = 16
      GroupIndex = 1
      Caption = 'h'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton23Click
    end
    object SpeedButton18: TSpeedButton
      Tag = 2
      Left = 45
      Top = 13
      Width = 20
      Height = 16
      GroupIndex = 1
      Caption = 'k'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton23Click
    end
    object SpeedButton19: TSpeedButton
      Tag = 3
      Left = 3
      Top = 30
      Width = 20
      Height = 16
      GroupIndex = 1
      Caption = 'f'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton23Click
    end
    object SpeedButton20: TSpeedButton
      Tag = 5
      Left = 45
      Top = 30
      Width = 20
      Height = 16
      GroupIndex = 1
      Caption = 'g'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton23Click
    end
    object SpeedButton21: TSpeedButton
      Tag = 6
      Left = 3
      Top = 47
      Width = 20
      Height = 16
      GroupIndex = 1
      Caption = 'l'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton23Click
    end
    object SpeedButton22: TSpeedButton
      Tag = 7
      Left = 24
      Top = 47
      Width = 20
      Height = 16
      GroupIndex = 1
      Caption = 'i'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton23Click
    end
    object SpeedButton23: TSpeedButton
      Tag = 8
      Left = 45
      Top = 47
      Width = 20
      Height = 16
      GroupIndex = 1
      Caption = 'm'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton23Click
    end
  end
  object GroupBox3: TGroupBox
    Left = 207
    Top = 70
    Width = 73
    Height = 30
    Caption = 'Brush size'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    object SpeedButton28: TSpeedButton
      Left = 28
      Top = 13
      Width = 20
      Height = 12
      Caption = #8211
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton28Click
      OnMouseEnter = SpeedButton28MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object SpeedButton30: TSpeedButton
      Left = 49
      Top = 13
      Width = 20
      Height = 12
      Caption = '+'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton30Click
      OnMouseEnter = SpeedButton30MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object Label1: TLabel
      Left = 8
      Top = 13
      Width = 10
      Height = 12
      Caption = 'L1'
    end
  end
  object GroupBox4: TGroupBox
    Left = 207
    Top = 99
    Width = 73
    Height = 54
    Caption = 'Snap size'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -10
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    object SpeedButton29: TSpeedButton
      Left = 28
      Top = 13
      Width = 20
      Height = 12
      Caption = #8211
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton29Click
      OnMouseEnter = SpeedButton29MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object SpeedButton31: TSpeedButton
      Left = 49
      Top = 13
      Width = 20
      Height = 12
      Caption = '+'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton31Click
      OnMouseEnter = SpeedButton31MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object Label2: TLabel
      Left = 5
      Top = 26
      Width = 10
      Height = 12
      Caption = 'L2'
    end
    object SpeedButton38: TSpeedButton
      Tag = 1
      Left = 49
      Top = 26
      Width = 20
      Height = 12
      Caption = '+'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton31Click
      OnMouseEnter = SpeedButton38MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object SpeedButton39: TSpeedButton
      Tag = 1
      Left = 28
      Top = 26
      Width = 20
      Height = 12
      Caption = #8211
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton29Click
      OnMouseEnter = SpeedButton39MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object Label3: TLabel
      Left = 5
      Top = 38
      Width = 10
      Height = 12
      Caption = 'L3'
    end
    object SpeedButton40: TSpeedButton
      Tag = 2
      Left = 49
      Top = 38
      Width = 20
      Height = 12
      Caption = '+'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton31Click
      OnMouseEnter = SpeedButton40MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object SpeedButton41: TSpeedButton
      Tag = 2
      Left = 28
      Top = 38
      Width = 20
      Height = 12
      Caption = #8211
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton29Click
      OnMouseEnter = SpeedButton41MouseEnter
      OnMouseLeave = SpeedButton24MouseLeave
    end
    object Label5: TLabel
      Left = 4
      Top = 13
      Width = 21
      Height = 12
      Caption = 'both'
    end
  end
  object BrushmaskTimer: TTimer
    Enabled = False
    Interval = 50
    OnTimer = BrushmaskTimerTimer
    Left = 10
    Top = 13
  end
  object StatTimer: TTimer
    Enabled = False
    Interval = 50
    OnTimer = StatTimerTimer
    Left = 40
    Top = 13
  end
  object PopupMenu1: TPopupMenu
    Left = 40
    Top = 47
    object Squares1: TMenuItem
      Caption = 'Squares'
      object Filledsquares281: TMenuItem
        Tag = 2
        Caption = 'Squares, filled 2-8'
        OnClick = Filledsquares281Click
      end
      object Filledsquares6121: TMenuItem
        Tag = 6
        Caption = 'Squares, filled 6-12'
        OnClick = Filledsquares281Click
      end
      object Filledsquares10161: TMenuItem
        Tag = 10
        Caption = 'Squares, filled 10-16'
        OnClick = Filledsquares281Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Outlinedsquares281: TMenuItem
        Tag = 3
        Caption = 'Squares, outlined 3-9'
        OnClick = Outlinedsquares281Click
      end
      object Squaresoutlined7131: TMenuItem
        Tag = 7
        Caption = 'Squares, outlined 7-13'
        OnClick = Outlinedsquares281Click
      end
      object Outlinedsquares10161: TMenuItem
        Tag = 10
        Caption = 'Squares, outlined 10-16'
        OnClick = Outlinedsquares281Click
      end
    end
    object Roundedsquares1: TMenuItem
      Caption = 'Rounded squares'
      object Roundedsquares4101: TMenuItem
        Tag = 4
        Caption = 'Rounded squares, filled 4-10'
        OnClick = Roundedsquares4101Click
      end
      object Roundedsquaresfilled7131: TMenuItem
        Tag = 7
        Caption = 'Rounded squares, filled 7-13'
        OnClick = Roundedsquares4101Click
      end
      object Roundedsquares10161: TMenuItem
        Tag = 10
        Caption = 'Rounded squares, filled 10-16'
        OnClick = Roundedsquares4101Click
      end
      object N10: TMenuItem
        Caption = '-'
      end
      object Roundedsquaresfilled4101: TMenuItem
        Tag = 4
        Caption = 'Rounded squares, outlined 4-10'
        OnClick = Roundedsquaresfilled4101Click
      end
      object Roundedsquaresoutlined4101: TMenuItem
        Tag = 7
        Caption = 'Rounded squares, outlined 7-13'
        OnClick = Roundedsquaresfilled4101Click
      end
      object Roundedsquaresoutlined10161: TMenuItem
        Tag = 10
        Caption = 'Rounded squares, outlined 10-16'
        OnClick = Roundedsquaresfilled4101Click
      end
      object N11: TMenuItem
        Caption = '-'
      end
      object Roundedsquaresjaggy4101: TMenuItem
        Tag = 4
        Caption = 'Rounded squares, jaggy 4-10'
        OnClick = Roundedsquaresjaggy4101Click
      end
      object Roundedsquaresjaggy7131: TMenuItem
        Tag = 7
        Caption = 'Rounded squares, jaggy 7-13'
        OnClick = Roundedsquaresjaggy4101Click
      end
      object Roundedsquaresjaggy10161: TMenuItem
        Tag = 10
        Caption = 'Rounded squares, jaggy 10-16'
        OnClick = Roundedsquaresjaggy4101Click
      end
    end
    object Circles1: TMenuItem
      Caption = 'Circles'
      object Circles281: TMenuItem
        Tag = 3
        Caption = 'Circles, filled 3-9'
        OnClick = Circles281Click
      end
      object Circlesfilled7131: TMenuItem
        Caption = 'Circles, filled 7-13'
        OnClick = Circles281Click
      end
      object Circles10161: TMenuItem
        Tag = 10
        Caption = 'Circles, filled 10-16'
        OnClick = Circles281Click
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object Circlesoutlined281: TMenuItem
        Tag = 3
        Caption = 'Circles, outlined 3-9'
        OnClick = Circlesoutlined281Click
      end
      object Circlesoutlined7131: TMenuItem
        Tag = 7
        Caption = 'Circles, outlined 7-13'
        OnClick = Circlesoutlined281Click
      end
      object Circlesoutlined10161: TMenuItem
        Tag = 10
        Caption = 'Circles, outlined 10-16'
        OnClick = Circlesoutlined281Click
      end
      object N8: TMenuItem
        Caption = '-'
      end
      object Circlesjaggy391: TMenuItem
        Tag = 3
        Caption = 'Circles, jaggy 3-9'
        OnClick = Circlesjaggy391Click
      end
      object Circlesjaggy7131: TMenuItem
        Tag = 7
        Caption = 'Circles, jaggy 7-13'
        OnClick = Circlesjaggy391Click
      end
      object Circlesjaggy10161: TMenuItem
        Tag = 10
        Caption = 'Circles, jaggy 10-16'
        OnClick = Circlesjaggy391Click
      end
    end
    object Diamonds1: TMenuItem
      Caption = 'Diamonds'
      object Diamondsfilled391: TMenuItem
        Tag = 3
        Caption = 'Diamonds, filled 3-9'
        OnClick = Diamondsfilled391Click
      end
      object Diamondsfilled7131: TMenuItem
        Tag = 7
        Caption = 'Diamonds, filled 7-13'
        OnClick = Diamondsfilled391Click
      end
      object Diamondsfilled10161: TMenuItem
        Tag = 10
        Caption = 'Diamonds, filled 10-16'
        OnClick = Diamondsfilled391Click
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object Diamondsoutlined391: TMenuItem
        Tag = 3
        Caption = 'Diamonds, outlined 3-9'
        OnClick = Diamondsoutlined391Click
      end
      object Diamondsoutlined7131: TMenuItem
        Tag = 7
        Caption = 'Diamonds, outlined 7-13'
        OnClick = Diamondsoutlined391Click
      end
      object Diamondsoutlined10161: TMenuItem
        Tag = 10
        Caption = 'Diamonds, outlined 10-16'
        OnClick = Diamondsoutlined391Click
      end
      object N9: TMenuItem
        Caption = '-'
      end
      object Diamondsjaggy391: TMenuItem
        Tag = 3
        Caption = 'Diamonds, jaggy 3-9'
        OnClick = Diamondsjaggy391Click
      end
      object Diamondsjaggy3131: TMenuItem
        Tag = 7
        Caption = 'Diamonds, jaggy 7-13'
        OnClick = Diamondsjaggy391Click
      end
      object Diamondsjaggy10161: TMenuItem
        Tag = 10
        Caption = 'Diamonds, jaggy 10-16'
        OnClick = Diamondsjaggy391Click
      end
    end
    object Wedges1: TMenuItem
      Caption = 'Wedges'
      object N901: TMenuItem
        Tag = 2
        Caption = '90-45-45'#176' wedges, filled 2-8'
        OnClick = N901Click
      end
      object N90wedgesfilled6121: TMenuItem
        Tag = 6
        Caption = '90-45-45'#176' wedges, filled 6-12'
        OnClick = N901Click
      end
      object N90wedgesfilled10161: TMenuItem
        Tag = 10
        Caption = '90-45-45'#176' wedges, filled 10-16'
        OnClick = N901Click
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object N90wedgesoutlined281: TMenuItem
        Tag = 4
        Caption = '90-45-45'#176' wedges, outlined 4-10'
        OnClick = N90wedgesoutlined281Click
      end
      object N904545wedgesoutline7131: TMenuItem
        Tag = 7
        Caption = '90-45-45'#176' wedges, outlined 7-13'
        OnClick = N90wedgesoutlined281Click
      end
      object N90wedgesoutlined10161: TMenuItem
        Tag = 10
        Caption = '90-45-45'#176' wedges, outlined 10-16'
        OnClick = N90wedgesoutlined281Click
      end
    end
    object Rightangleswedges1: TMenuItem
      Caption = 'Lines'
      object Linesat7angles8x81: TMenuItem
        Tag = 8
        Caption = 'Lines at 7 angles, 8x8'
        OnClick = Linesat7angles8x81Click
      end
      object Linesat7angles16x161: TMenuItem
        Tag = 16
        Caption = 'Lines at 7 angles, 16x16'
        OnClick = Linesat7angles8x81Click
      end
      object N12: TMenuItem
        Caption = '-'
      end
      object Linesat7anglesjaggy8x81: TMenuItem
        Tag = 8
        Caption = 'Jaggy lines at 7 angles, 8x8'
        OnClick = Linesat7anglesjaggy8x81Click
      end
      object Linesat7anglesjaggy16x161: TMenuItem
        Tag = 16
        Caption = 'Jaggy lines at 7 angles, 16x16'
        OnClick = Linesat7anglesjaggy8x81Click
      end
    end
  end
  object PopupMenu2: TPopupMenu
    Left = 6
    Top = 59
    object Fromsolidpixels1: TMenuItem
      Caption = 'From solid pixels'
      OnClick = Fromsolidpixels1Click
    end
    object N6: TMenuItem
      Caption = '-'
    end
    object Fromcolour01: TMenuItem
      Tag = 1
      Caption = 'From bitplane 0'
      OnClick = Fromsolidpixels1Click
    end
    object Frombitplane11: TMenuItem
      Tag = 2
      Caption = 'From bitplane 1'
      OnClick = Fromsolidpixels1Click
    end
    object N7: TMenuItem
      Caption = '-'
    end
    object Fromcolour11: TMenuItem
      Tag = 3
      Caption = 'From colour 0'
      OnClick = Fromsolidpixels1Click
    end
    object Fromcolour12: TMenuItem
      Tag = 4
      Caption = 'From colour 1'
      OnClick = Fromsolidpixels1Click
    end
    object Fromcolour21: TMenuItem
      Tag = 5
      Caption = 'From colour 2'
      OnClick = Fromsolidpixels1Click
    end
    object Fromcolour31: TMenuItem
      Tag = 6
      Caption = 'From colour 3'
      OnClick = Fromsolidpixels1Click
    end
  end
  object PopupMenu3: TPopupMenu
    Left = 64
    Top = 76
    object Currentset2: TMenuItem
      Tag = 1
      Caption = 'Current set...'
      OnClick = Currentsinglebrush1Click
    end
    object Doubleset2: TMenuItem
      Tag = 2
      Caption = 'Double set...'
      OnClick = Currentsinglebrush1Click
    end
    object N13: TMenuItem
      Caption = '-'
    end
    object Currentsinglebrush2: TMenuItem
      Caption = 'Current single brush...'
      OnClick = Currentsinglebrush1Click
    end
  end
  object TimerRotate1: TTimer
    Enabled = False
    Interval = 50
    OnTimer = TimerRotate1Timer
    Left = 72
    Top = 16
  end
  object TimerRestoreCaption: TTimer
    Enabled = False
    OnTimer = TimerRestoreCaptionTimer
    Top = 88
  end
end
