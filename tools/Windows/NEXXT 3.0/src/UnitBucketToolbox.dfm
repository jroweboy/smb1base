object FormBucketToolbox: TFormBucketToolbox
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Bucket toolbox'
  ClientHeight = 309
  ClientWidth = 157
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PopupMode = pmExplicit
  Position = poDesigned
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox2: TGroupBox
    Left = 3
    Top = 3
    Width = 153
    Height = 38
    Caption = 'Default mode (bound to ctrl)'
    TabOrder = 1
    object btnClassic: TSpeedButton
      Left = 3
      Top = 19
      Width = 71
      Height = 16
      GroupIndex = 1
      Down = True
      Caption = 'Classic'
      OnMouseDown = btnClassicMouseDown
      OnMouseEnter = btnClassicMouseEnter
    end
    object btnForgiving: TSpeedButton
      Left = 79
      Top = 17
      Width = 71
      Height = 16
      GroupIndex = 1
      Caption = 'Gap Aware'
      OnMouseDown = btnForgivingMouseDown
      OnMouseEnter = btnForgivingMouseEnter
    end
  end
  object GroupBox1: TGroupBox
    Left = 3
    Top = 100
    Width = 153
    Height = 134
    Caption = 'Gap Aware:'
    TabOrder = 0
    object Label1: TLabel
      Left = 7
      Top = 20
      Width = 60
      Height = 11
      Caption = 'Gap detection:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object btnSides: TSpeedButton
      Left = 3
      Top = 38
      Width = 71
      Height = 16
      AllowAllUp = True
      GroupIndex = 10
      Down = True
      Caption = 'Laterals'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnSidesClick
      OnMouseEnter = btnSidesMouseEnter
    end
    object btnSemis: TSpeedButton
      Left = 3
      Top = 55
      Width = 71
      Height = 16
      AllowAllUp = True
      GroupIndex = 11
      Down = True
      Caption = 'Semi-diagonals'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnSidesClick
      OnMouseEnter = btnSemisMouseEnter
    end
    object btnDiags: TSpeedButton
      Left = 3
      Top = 72
      Width = 71
      Height = 16
      AllowAllUp = True
      GroupIndex = 12
      Down = True
      Caption = 'Diagonals'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnSidesClick
      OnMouseEnter = btnDiagsMouseEnter
    end
    object Label2: TLabel
      Left = 88
      Top = 20
      Width = 51
      Height = 11
      Caption = 'Flood areas:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object btnFields: TSpeedButton
      Left = 78
      Top = 38
      Width = 71
      Height = 16
      GroupIndex = 1
      Caption = 'Field'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btnFieldsMouseEnter
    end
    object btnGaps: TSpeedButton
      Left = 78
      Top = 55
      Width = 71
      Height = 16
      GroupIndex = 1
      Caption = 'Gaps'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btnGapsMouseEnter
    end
    object btnBoth: TSpeedButton
      Left = 78
      Top = 72
      Width = 71
      Height = 16
      GroupIndex = 1
      Down = True
      Caption = 'Both'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btnBothMouseEnter
    end
    object Label4: TLabel
      Left = 3
      Top = 113
      Width = 47
      Height = 11
      Caption = 'Gap colour:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object btnGapPen: TSpeedButton
      Left = 56
      Top = 113
      Width = 46
      Height = 16
      GroupIndex = 2
      Down = True
      Caption = 'Pen'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btnGapPenMouseEnter
    end
    object SpeedButton12: TSpeedButton
      Left = 104
      Top = 113
      Width = 46
      Height = 16
      GroupIndex = 2
      Caption = 'Border'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = SpeedButton12MouseEnter
    end
    object Label3: TLabel
      Left = 4
      Top = 132
      Width = 46
      Height = 11
      Caption = 'Gap width:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      Visible = False
    end
    object SpeedButton15: TSpeedButton
      Left = 56
      Top = 132
      Width = 46
      Height = 16
      AllowAllUp = True
      GroupIndex = 20
      Down = True
      Caption = '1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      Visible = False
      OnMouseUp = SpeedButton15MouseUp
    end
    object SpeedButton16: TSpeedButton
      Left = 104
      Top = 132
      Width = 46
      Height = 16
      AllowAllUp = True
      GroupIndex = 21
      Caption = '2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      Visible = False
      OnMouseUp = SpeedButton15MouseUp
    end
    object Label6: TLabel
      Left = 11
      Top = 94
      Width = 39
      Height = 11
      Caption = 'Direction:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object btnSmartAll: TSpeedButton
      Left = 56
      Top = 94
      Width = 46
      Height = 16
      GroupIndex = 3
      Down = True
      Caption = '4-way'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btnSmartAllMouseEnter
    end
    object btnSmartCustom: TSpeedButton
      Left = 104
      Top = 94
      Width = 46
      Height = 16
      GroupIndex = 3
      Caption = 'Custom'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btnSmartCustomMouseEnter
    end
  end
  object GroupBox3: TGroupBox
    Left = 2
    Top = 350
    Width = 153
    Height = 38
    Caption = 'Flood boundaries'
    TabOrder = 2
    Visible = False
    object SpeedButton13: TSpeedButton
      Left = 3
      Top = 17
      Width = 71
      Height = 16
      GroupIndex = 2
      Caption = 'Tile'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object SpeedButton14: TSpeedButton
      Left = 79
      Top = 17
      Width = 71
      Height = 16
      GroupIndex = 2
      Down = True
      Caption = 'Selection'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
  end
  object GroupBox4: TGroupBox
    Left = 3
    Top = 40
    Width = 153
    Height = 61
    Caption = 'Classic fill:'
    TabOrder = 3
    object btn4way: TSpeedButton
      Left = 3
      Top = 18
      Width = 48
      Height = 16
      GroupIndex = 1
      Down = True
      Caption = '4-way'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btn4wayMouseEnter
    end
    object btn8way: TSpeedButton
      Left = 52
      Top = 18
      Width = 48
      Height = 16
      GroupIndex = 1
      Caption = '8-way'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btn8wayMouseEnter
    end
    object Label5: TLabel
      Left = 3
      Top = 40
      Width = 50
      Height = 11
      Caption = 'Field colour:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object btnFieldPen: TSpeedButton
      Left = 56
      Top = 40
      Width = 46
      Height = 16
      GroupIndex = 2
      Down = True
      Caption = 'Pen'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btnFieldPenMouseEnter
    end
    object SpeedButton10: TSpeedButton
      Left = 104
      Top = 40
      Width = 46
      Height = 16
      GroupIndex = 2
      Caption = 'Border'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = SpeedButton10MouseEnter
    end
    object btnCustomway: TSpeedButton
      Left = 102
      Top = 18
      Width = 48
      Height = 16
      GroupIndex = 1
      Caption = 'Custom'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btnCustomwayMouseEnter
    end
  end
  object GroupBox5: TGroupBox
    Left = 2
    Top = 234
    Width = 154
    Height = 74
    Caption = 'Custom flood direction'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    object btnC_nw: TSpeedButton
      Left = 78
      Top = 14
      Width = 23
      Height = 16
      AllowAllUp = True
      GroupIndex = 1
      Caption = 'j'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnMouseDown = btnC_nwMouseDown
      OnMouseEnter = btnC_nwMouseEnter
    end
    object btnSwitchDir: TSpeedButton
      Tag = 4
      Left = 103
      Top = 33
      Width = 23
      Height = 16
      AllowAllUp = True
      Caption = 'A'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseDown = btnSwitchDirMouseDown
      OnMouseEnter = btnSwitchDirMouseEnter
    end
    object btnC_n: TSpeedButton
      Tag = 1
      Left = 103
      Top = 14
      Width = 23
      Height = 16
      AllowAllUp = True
      GroupIndex = 2
      Caption = 'h'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnMouseDown = btnC_nMouseDown
      OnMouseEnter = btnC_nMouseEnter
    end
    object btnC_ne: TSpeedButton
      Tag = 2
      Left = 128
      Top = 14
      Width = 23
      Height = 16
      AllowAllUp = True
      GroupIndex = 3
      Caption = 'k'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnMouseDown = btnC_neMouseDown
      OnMouseEnter = btnC_neMouseEnter
    end
    object btnC_w: TSpeedButton
      Tag = 3
      Left = 78
      Top = 33
      Width = 23
      Height = 16
      AllowAllUp = True
      GroupIndex = 4
      Caption = 'f'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnMouseDown = btnC_wMouseDown
      OnMouseEnter = btnC_wMouseEnter
    end
    object btnC_e: TSpeedButton
      Tag = 5
      Left = 128
      Top = 33
      Width = 23
      Height = 16
      AllowAllUp = True
      GroupIndex = 6
      Caption = 'g'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnMouseDown = btnC_eMouseDown
      OnMouseEnter = btnC_eMouseEnter
    end
    object btnC_sw: TSpeedButton
      Tag = 6
      Left = 78
      Top = 52
      Width = 23
      Height = 16
      AllowAllUp = True
      GroupIndex = 7
      Caption = 'l'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnMouseDown = btnC_swMouseDown
      OnMouseEnter = btnC_swMouseEnter
    end
    object btnC_s: TSpeedButton
      Tag = 7
      Left = 103
      Top = 52
      Width = 23
      Height = 16
      AllowAllUp = True
      GroupIndex = 8
      Caption = 'i'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnMouseDown = btnC_sMouseDown
      OnMouseEnter = btnC_sMouseEnter
    end
    object btnC_se: TSpeedButton
      Tag = 8
      Left = 128
      Top = 52
      Width = 23
      Height = 16
      AllowAllUp = True
      GroupIndex = 9
      Caption = 'm'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnMouseDown = btnC_seMouseDown
      OnMouseEnter = btnC_seMouseEnter
    end
    object SpeedButton24: TSpeedButton
      Left = 3
      Top = 52
      Width = 35
      Height = 16
      Caption = 'D'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -14
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnMouseDown = SpeedButton24MouseDown
      OnMouseEnter = SpeedButton24MouseEnter
    end
    object SpeedButton25: TSpeedButton
      Left = 40
      Top = 52
      Width = 35
      Height = 16
      Caption = 'E'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -14
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnMouseDown = SpeedButton25MouseDown
      OnMouseEnter = SpeedButton25MouseEnter
    end
    object SpeedButton27: TSpeedButton
      Left = 3
      Top = 33
      Width = 35
      Height = 16
      Caption = 'Q'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -14
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnMouseDown = SpeedButton27MouseDown
      OnMouseEnter = SpeedButton27MouseEnter
    end
    object SpeedButton26: TSpeedButton
      Left = 40
      Top = 33
      Width = 35
      Height = 16
      Caption = 'P'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -14
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnMouseDown = SpeedButton26MouseDown
      OnMouseEnter = SpeedButton26MouseEnter
    end
    object SpeedButton5: TSpeedButton
      Tag = 4
      Left = 3
      Top = 14
      Width = 35
      Height = 16
      AllowAllUp = True
      Caption = 'invert'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseDown = SpeedButton5MouseDown
      OnMouseEnter = SpeedButton5MouseEnter
    end
    object btnForceBuf: TSpeedButton
      Tag = 4
      Left = 40
      Top = 14
      Width = 35
      Height = 16
      AllowAllUp = True
      GroupIndex = 20
      Down = True
      Caption = 'buffer'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btnForceBufMouseEnter
    end
  end
end
