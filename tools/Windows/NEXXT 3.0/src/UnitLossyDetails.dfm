object FormLossyDetails: TFormLossyDetails
  Left = 0
  Top = 0
  BorderStyle = bsSizeToolWin
  Caption = 'Tile matcher'
  ClientHeight = 287
  ClientWidth = 203
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object SpeedButton4: TSpeedButton
    Left = 159
    Top = 211
    Width = 40
    Height = 31
    Caption = 'Run'
  end
  object GroupBox1: TGroupBox
    Left = 4
    Top = 97
    Width = 152
    Height = 59
    Caption = 'By position'
    TabOrder = 0
    object CheckBox3: TCheckBox
      Left = 11
      Top = 15
      Width = 51
      Height = 17
      Caption = 'Inland'
      TabOrder = 0
    end
    object CheckBox6: TCheckBox
      Left = 11
      Top = 34
      Width = 51
      Height = 17
      Caption = 'Edge'
      TabOrder = 1
    end
    object TrackBar5: TTrackBar
      Left = 60
      Top = 13
      Width = 90
      Height = 20
      Max = 20
      PageSize = 1
      Position = 10
      TabOrder = 2
      ThumbLength = 7
      TickMarks = tmTopLeft
    end
    object TrackBar6: TTrackBar
      Left = 60
      Top = 32
      Width = 90
      Height = 20
      Max = 20
      PageSize = 1
      Position = 10
      TabOrder = 3
      ThumbLength = 7
      TickMarks = tmTopLeft
    end
  end
  object GroupBox2: TGroupBox
    Left = 4
    Top = 4
    Width = 152
    Height = 93
    Caption = 'By histogram intersection'
    TabOrder = 1
    object CheckBox1: TCheckBox
      Left = 11
      Top = 15
      Width = 42
      Height = 17
      Caption = 'col 0'
      Checked = True
      State = cbChecked
      TabOrder = 0
    end
    object CheckBox2: TCheckBox
      Left = 11
      Top = 34
      Width = 43
      Height = 17
      Caption = 'col 1'
      Checked = True
      State = cbChecked
      TabOrder = 1
    end
    object CheckBox4: TCheckBox
      Left = 11
      Top = 53
      Width = 40
      Height = 17
      Caption = 'col 2'
      Checked = True
      State = cbChecked
      TabOrder = 2
    end
    object CheckBox5: TCheckBox
      Left = 11
      Top = 72
      Width = 40
      Height = 17
      Caption = 'col 3'
      Checked = True
      State = cbChecked
      TabOrder = 3
    end
    object TrackBar1: TTrackBar
      Left = 60
      Top = 11
      Width = 90
      Height = 20
      Max = 20
      PageSize = 1
      Position = 10
      TabOrder = 4
      ThumbLength = 7
      TickMarks = tmTopLeft
    end
    object TrackBar2: TTrackBar
      Left = 60
      Top = 29
      Width = 90
      Height = 20
      Max = 20
      PageSize = 1
      Position = 10
      TabOrder = 5
      ThumbLength = 7
      TickMarks = tmTopLeft
    end
    object TrackBar3: TTrackBar
      Left = 60
      Top = 49
      Width = 90
      Height = 20
      Max = 20
      PageSize = 1
      Position = 10
      TabOrder = 6
      ThumbLength = 7
      TickMarks = tmTopLeft
    end
    object TrackBar4: TTrackBar
      Left = 60
      Top = 69
      Width = 90
      Height = 20
      Max = 20
      PageSize = 1
      Position = 10
      TabOrder = 7
      ThumbLength = 7
      TickMarks = tmTopLeft
    end
  end
  object GroupBox3: TGroupBox
    Left = 4
    Top = 156
    Width = 111
    Height = 50
    Caption = 'Squaremean results'
    TabOrder = 2
    object CheckBox7: TCheckBox
      Left = 11
      Top = 14
      Width = 89
      Height = 17
      Caption = 'Histogram'
      TabOrder = 0
    end
    object CheckBox8: TCheckBox
      Left = 11
      Top = 30
      Width = 56
      Height = 17
      Caption = 'Position'
      TabOrder = 1
    end
  end
  object GroupBox4: TGroupBox
    Left = 121
    Top = 156
    Width = 79
    Height = 50
    Caption = 'Max tiles'
    TabOrder = 3
    object Edit1: TEdit
      Left = 14
      Top = 16
      Width = 36
      Height = 21
      TabOrder = 0
      Text = '0'
    end
    object UpDown1: TUpDown
      Left = 56
      Top = 16
      Width = 15
      Height = 21
      Associate = Edit1
      TabOrder = 1
    end
  end
  object GroupBox5: TGroupBox
    Left = 160
    Top = 4
    Width = 40
    Height = 93
    Caption = 'Score'
    TabOrder = 4
    object Label3: TLabel
      Left = 11
      Top = 15
      Width = 18
      Height = 13
      Caption = 'LC0'
      FocusControl = TrackBar1
    end
    object Label4: TLabel
      Left = 11
      Top = 34
      Width = 18
      Height = 13
      Caption = 'LC1'
    end
    object Label5: TLabel
      Left = 11
      Top = 53
      Width = 18
      Height = 13
      Caption = 'LC2'
    end
    object Label6: TLabel
      Left = 11
      Top = 72
      Width = 18
      Height = 13
      Caption = 'LC3'
    end
  end
  object GroupBox6: TGroupBox
    Left = 160
    Top = 97
    Width = 40
    Height = 59
    Caption = 'Score'
    TabOrder = 5
    object Label7: TLabel
      Left = 11
      Top = 15
      Width = 16
      Height = 13
      Caption = 'LBL'
    end
    object Label8: TLabel
      Left = 11
      Top = 34
      Width = 18
      Height = 13
      Caption = 'LBH'
    end
  end
  object GroupBox8: TGroupBox
    Left = 4
    Top = 244
    Width = 196
    Height = 41
    Caption = 'Presets'
    TabOrder = 6
    object SpeedButton1: TSpeedButton
      Left = 8
      Top = 14
      Width = 30
      Height = 20
      Caption = '1'
    end
    object SpeedButton2: TSpeedButton
      Left = 39
      Top = 14
      Width = 30
      Height = 20
      Caption = '2'
    end
    object SpeedButton3: TSpeedButton
      Left = 70
      Top = 14
      Width = 30
      Height = 20
      Caption = '3'
    end
    object Button1: TButton
      Left = 146
      Top = 14
      Width = 42
      Height = 20
      Caption = 'Reset'
      TabOrder = 0
    end
    object Button2: TButton
      Left = 105
      Top = 14
      Width = 40
      Height = 20
      Caption = 'Set'
      TabOrder = 1
    end
  end
  object GroupBox7: TGroupBox
    Left = 4
    Top = 206
    Width = 150
    Height = 36
    Caption = 'Mix'
    TabOrder = 7
    object Label1: TLabel
      Left = 8
      Top = 14
      Width = 17
      Height = 13
      Caption = 'Pos'
    end
    object Label2: TLabel
      Left = 126
      Top = 14
      Width = 15
      Height = 13
      Caption = 'Col'
    end
    object TrackBar7: TTrackBar
      Left = 30
      Top = 8
      Width = 90
      Height = 20
      Max = 20
      PageSize = 1
      Position = 10
      TabOrder = 0
      ThumbLength = 7
      TickMarks = tmTopLeft
    end
  end
  object CheckBox9: TCheckBox
    Left = 137
    Top = 3
    Width = 14
    Height = 17
    TabOrder = 8
  end
  object CheckBox10: TCheckBox
    Left = 137
    Top = 96
    Width = 14
    Height = 17
    TabOrder = 9
  end
end
