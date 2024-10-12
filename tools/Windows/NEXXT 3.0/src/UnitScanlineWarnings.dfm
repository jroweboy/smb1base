object FormScanlineWarnings: TFormScanlineWarnings
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Scanline warnings'
  ClientHeight = 107
  ClientWidth = 347
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnKeyPress = FormKeyPress
  OnKeyUp = FormKeyUp
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 2
    Top = 0
    Width = 112
    Height = 106
    Caption = 'Metasprite tab'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object btnCyan: TSpeedButton
      Left = 5
      Top = 13
      Width = 68
      Height = 18
      AllowAllUp = True
      GroupIndex = 4
      Caption = 'Custom (cyan)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnCyanClick
      OnMouseEnter = btnCyanMouseEnter
    end
    object Label1: TLabel
      Left = 76
      Top = 13
      Width = 7
      Height = 16
      Caption = '3'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object btnRed: TSpeedButton
      Left = 5
      Top = 83
      Width = 38
      Height = 18
      AllowAllUp = True
      GroupIndex = 3
      Caption = 'Red'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnCyanClick
      OnMouseEnter = btnRedMouseEnter
    end
    object btnYellow: TSpeedButton
      Left = 5
      Top = 36
      Width = 38
      Height = 18
      AllowAllUp = True
      GroupIndex = 1
      Caption = 'Yellow'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnCyanClick
      OnMouseEnter = btnYellowMouseEnter
    end
    object btnOrange: TSpeedButton
      Left = 5
      Top = 59
      Width = 38
      Height = 18
      AllowAllUp = True
      GroupIndex = 2
      Caption = 'Orange'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnCyanClick
      OnMouseEnter = btnOrangeMouseEnter
    end
    object Label10: TLabel
      Left = 46
      Top = 81
      Width = 62
      Height = 22
      Caption = 'Sprites exceed scanline limit'
      WordWrap = True
    end
    object Label11: TLabel
      Left = 46
      Top = 56
      Width = 52
      Height = 22
      Caption = 'Sprites at scanline limit'
      WordWrap = True
    end
    object Label12: TLabel
      Left = 46
      Top = 33
      Width = 60
      Height = 22
      Caption = 'More than half of scanline lmt'
      WordWrap = True
    end
    object UpDown1: TUpDown
      Left = 90
      Top = 13
      Width = 16
      Height = 17
      Min = 1
      Max = 16
      Position = 1
      TabOrder = 0
      Wrap = True
      OnChanging = UpDown1Changing
    end
  end
  object GroupBox2: TGroupBox
    Left = 114
    Top = 0
    Width = 230
    Height = 106
    Caption = 'NTSC overscan guide - safety margin'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object Label2: TLabel
      Left = 5
      Top = 22
      Width = 22
      Height = 13
      Caption = 'Top:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 5
      Top = 76
      Width = 66
      Height = 22
      Caption = 'Idealized case    (224 scanlines)'
      WordWrap = True
    end
    object Label4: TLabel
      Left = 208
      Top = 22
      Width = 12
      Height = 13
      Caption = '16'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label5: TLabel
      Left = 124
      Top = 76
      Width = 100
      Height = 22
      Alignment = taRightJustify
      Caption = 'Account for a reasonable worst case'
      WordWrap = True
    end
    object Label6: TLabel
      Left = 5
      Top = 50
      Width = 38
      Height = 13
      Caption = 'Bottom:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label7: TLabel
      Left = 208
      Top = 50
      Width = 12
      Height = 13
      Caption = '12'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label8: TLabel
      Left = 60
      Top = 22
      Width = 6
      Height = 13
      Caption = '8'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label9: TLabel
      Left = 60
      Top = 48
      Width = 6
      Height = 13
      Caption = '8'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object TrkTop: TTrackBar
      Left = 70
      Top = 20
      Width = 135
      Height = 20
      Max = 16
      Min = 8
      PageSize = 1
      Position = 16
      TabOrder = 0
      TabStop = False
      ThumbLength = 10
      TickMarks = tmTopLeft
      OnChange = TrkTopChange
    end
    object TrkBottom: TTrackBar
      Left = 70
      Top = 48
      Width = 135
      Height = 20
      Max = 12
      Min = 8
      PageSize = 1
      Position = 12
      TabOrder = 1
      TabStop = False
      ThumbLength = 10
      TickMarks = tmTopLeft
      OnChange = TrkTopChange
    end
  end
end
